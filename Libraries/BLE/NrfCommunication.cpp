#include "NrfCommunication.hpp"
#include "spiCommonConfig.h"
#include "AutoLock.h"
#include "timing_macros.h"
#include <algorithm>
#include "assertTools.h"
#include "FreeRTOSUtils.hpp"
#include "strlen.h"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "nrf"

static const char RX_PROTOCOL_SOF = '\x01';
static const char RX_PROTOCOL_EOF = '\x04';
static const uint8_t RX_PROTOCOL_SOF_IDX = 0;
static const uint8_t RX_PROTOCOL_FRAME_LEN_IDX = 1;
static const uint8_t RX_PROTOCOL_DATA_LEN_START_IDX = 2;
static const uint8_t RX_PROTOCOL_DATA_LEN_END_IDX = 3;
static const uint8_t RX_PROTOCOL_CMD_ID_IDX = 4;
static const uint8_t RX_PROTOCOL_DATA_OFS_START_IDX = 5;
static const uint8_t RX_PROTOCOL_DATA_OFS_END_IDX = 6;
static const uint8_t RX_PROTOCOL_DATA_START_IDX = 7;

SPIConfig NrfCommunication::gBleCommunicationSPIConfig = gSPICommonConfig;
const uint8_t LENGTH_MAX_DATA = 6;

NrfCommunication::NrfCommunication(iSPI& iSPI,
                                   IGPIOController& gpioController,
                                   BLEIO& bleIO,
                                   IWorkQueue& workQueue)
    : mChipSelector(gpioController, bleIO.csN)
    , mISPI(iSPI)
    , mGPIOController(gpioController)
    , mBLEIO(bleIO)
    , mITCommStrategy(mGPIOController, mBLEIO.hrdy, mBLEIO.dts, *this)
    , mCommStrategy(mITCommStrategy)
    , mWorkQueue(workQueue)
    , mRequestTimeoutWork([this](etl::array_view<uint8_t> data) { onRequestTimeout(data); })
{
    // Initialize SPI
    mISPI.init(mSPIConfig);

    mQueue = xQueueCreate(15, sizeof(Message));

    ASSERT(pdPASS
           == xTaskCreate(vNrfCommunicationTask,
                          "NrfCommunicationTask",
                          BLE_TASK_STACK_SIZE,
                          this,
                          BLE_TASK_PRIORITY,
                          &mHandle));

    initDummyTxBuffer();

    resetRxProtocolDataParsing();
}

BLEDefs::ErrCode NrfCommunication::init()
{
    Message message = {};
    message.msgId = MessageId::INIT;

    BLEDefs::ErrCode ret = xQueueSendToBack(mQueue, (void*) &message, MSG_TIMEOUT)
                               ? BLEDefs::ErrCode::SUCCESS
                               : BLEDefs::ErrCode::BUSY;

    return ret;
}

BLEDefs::ErrCode NrfCommunication::sendCommand(BleTxCmd& cmd)
{
    BleCmdPacket* packet = allocatePacketFromPool();

    if(!packet)
    {
        BLEDefs::ErrCode ret = BLEDefs::ErrCode::BUSY;
        LOG_ERR(MODULE,
                "Cmd " << static_cast<uint8_t>(cmd.getId()) << "packet alloc error "
                       << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                             static_cast<uint16_t>(ret)));

        return ret;
    }

    cmd.fill(*packet);

    // Commands with no parameters need a dummy parameter
    if(packet->getParameters().size() == 0)
    {
        uint8_t dummyParameter = 0;
        packet->addParameter(BleCmdPacket::Parameter{&dummyParameter, sizeof(dummyParameter)});
    }

    Message message = {};
    message.msgId = MessageId::SEND_COMMAND;
    message.sendCommandArg.packet = packet;

    BLEDefs::ErrCode ret = (xQueueSendToBack(mQueue, &message, MSG_TIMEOUT) == pdTRUE)
                               ? BLEDefs::ErrCode::SUCCESS
                               : BLEDefs::ErrCode::BUSY;

    if(ret != BLEDefs::ErrCode::SUCCESS)
    {
        releasePacketToPool(packet);
        LOG_ERR(MODULE,
                "Command " << static_cast<uint8_t>(cmd.getId()) << " error "
                           << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                 static_cast<uint16_t>(ret)));
    }

    return ret;
}

BLEDefs::ErrCode NrfCommunication::sendRequest(BleTxRequest& cmd)
{
    BleRequestTimer* timer = allocateRequestTimerFromPool(cmd);

    if(!timer)
    {
        BLEDefs::ErrCode ret = BLEDefs::ErrCode::BUSY;
        LOG_ERR(MODULE,
                "Request " << static_cast<uint8_t>(cmd.getId()) << " timer alloc error "
                           << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                 static_cast<uint16_t>(ret)));
        return ret;
    }

    registerRequestListener(cmd, timer);

    BLEDefs::ErrCode ret = sendCommand(cmd);

    if(ret != BLEDefs::ErrCode::SUCCESS)
    {
        unregisterListener(cmd);
        LOG_ERR(MODULE,
                "Request " << static_cast<uint8_t>(cmd.getId()) << " error "
                           << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                 static_cast<uint16_t>(ret)));
    }
    else
    {
        timer->add_observer(*this);
        timer->start();
    }

    return ret;
}


BLEDefs::ErrCode NrfCommunication::spiReceive()
{
    BLEDefs::ErrCode errCode = BLEDefs::ErrCode::SUCCESS;

    errCode = spiTransmitReceiveSingle(mDummyTxBuffer, MAX_TRANSFER_LEN);

    if(errCode != BLEDefs::ErrCode::SUCCESS)
    {
        return BLEDefs::ErrCode::TRANSFER_FAILED;
    }

    processRxData(mRxBuffer, MAX_TRANSFER_LEN);

    return BLEDefs::ErrCode::SUCCESS;
}

BLEDefs::ErrCode NrfCommunication::spiTransmitReceive(const uint8_t* txData, size_t length)
{
    BLEDefs::ErrCode errCode = BLEDefs::ErrCode::SUCCESS;

    while(length > 0 && BLEDefs::ErrCode::SUCCESS == errCode)
    {
        size_t transferSize = std::min(length, MAX_TRANSFER_LEN);

        errCode = spiTransmitReceiveSingle(txData, transferSize);

        if(BLEDefs::ErrCode::SUCCESS == errCode)
        {
            processRxData(mRxBuffer, length);

            length -= transferSize;
            txData += transferSize;
        }
    }

    return errCode;
}

BLEDefs::ErrCode NrfCommunication::spiTransmitReceiveSingle(const uint8_t* txData, size_t length)
{
    if(length > MAX_TRANSFER_LEN)
    {
        return BLEDefs::ErrCode::INVALID_PARAMETER;
    }

    BLEDefs::ErrCode errCode = BLEDefs::ErrCode::SUCCESS;

    if(!mCommStrategy.get().waitForHostReady(DelayMs(WAIT_HOST_READY_DELAY_MS)))
    {
        LOG_ERR(MODULE, "Host not ready");
        errCode = BLEDefs::ErrCode::TRANSFER_FAILED;
    }
    else
    {
        ISPIErrCode spiErrCode = mISPI.transmitReceive(txData, mRxBuffer, length, mChipSelector);

        // MOSI state is now undetermined but is used by nRF to enter DFU mode
        // Clear MOSI to avoid entering DFU mode
        mISPI.clearMosi();

        if(ISPI_ERROR_SUCCESS != spiErrCode)
        {
            errCode = BLEDefs::ErrCode::TRANSFER_FAILED;
        }
    }

    if(errCode != BLEDefs::ErrCode::SUCCESS)
    {
        LOG_ERR(MODULE,
                "SPI TX/RX error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(errCode)));
    }

    return errCode;
}

bool NrfCommunication::isRxDataTransferInProgress() const
{
    return mRxCurrentDataLength != 0;
}

bool NrfCommunication::processRxDataParseHeaderByte(uint8_t data)
{
    bool bRet = true;

    if((mRxCurrentFrameOffset == RX_PROTOCOL_SOF_IDX) && (data == RX_PROTOCOL_SOF))
    {
        /* Nothing to do */
    }
    else if(mRxCurrentFrameOffset == RX_PROTOCOL_FRAME_LEN_IDX)
    {
        mRxCurrentFrameLength = data;
    }
    else if(mRxCurrentFrameOffset >= RX_PROTOCOL_DATA_LEN_START_IDX
            && mRxCurrentFrameOffset <= RX_PROTOCOL_DATA_LEN_END_IDX)
    {
        mRxCurrentDataLengthFromHeader |=
            (data << ((RX_PROTOCOL_DATA_LEN_END_IDX - mRxCurrentFrameOffset) * 8));
    }
    else if(mRxCurrentFrameOffset == RX_PROTOCOL_CMD_ID_IDX)
    {
        mRxCurrentCommandIdFromHeader = static_cast<BLESPICommands::CommandId>(data);
    }
    else if(mRxCurrentFrameOffset >= RX_PROTOCOL_DATA_OFS_START_IDX
            && mRxCurrentFrameOffset <= RX_PROTOCOL_DATA_OFS_END_IDX)
    {
        mRxCurrentDataOffsetFromHeader |=
            (data << ((RX_PROTOCOL_DATA_OFS_END_IDX - mRxCurrentFrameOffset) * 8));

        if(mRxCurrentFrameOffset == RX_PROTOCOL_DATA_OFS_END_IDX)
        {
            uint8_t payloadLen = mRxCurrentFrameLength
                                 - (RX_PROTOCOL_DATA_START_IDX - RX_PROTOCOL_FRAME_LEN_IDX);

            bool bValidParams = (mRxCurrentDataLengthFromHeader != 0)
                                && (mRxCurrentCommandIdFromHeader
                                    != BLESPICommands::CommandId::INVALID_CMD)
                                && (mRxCurrentDataOffsetFromHeader + payloadLen
                                    <= mRxCurrentDataLengthFromHeader);

            if(isRxDataTransferInProgress())
            {
                bValidParams &= (mRxCurrentDataLength == mRxCurrentDataLengthFromHeader)
                                && (mRxCurrentCommandId == mRxCurrentCommandIdFromHeader)
                                && (mRxCurrentDataOffset == mRxCurrentDataOffsetFromHeader);
            }
            else if(bValidParams)
            {
                mRxCurrentDataLength = mRxCurrentDataLengthFromHeader;
                mRxCurrentCommandId = mRxCurrentCommandIdFromHeader;
            }

            if(bValidParams)
            {
                mFrameRxParseState = RXPARSESTATE_DATA;
            }
            else
            {
                bRet = false;
            }
        }
    }
    else
    {
        bRet = false;
    }

    return bRet;
}

NrfCommunication::CommandListenerContext NrfCommunication::findBleCommandListenerContext(
    BLESPICommands::CommandId commandId)
{
    AutoLock autoLock(mListenersMutex);
    CommandListenerContext listenerContext = {nullptr, nullptr};

    if(auto ite = std::find_if(mCommandListenerContexts.begin(),
                               mCommandListenerContexts.end(),
                               [commandId](auto& commandListenerContext)
                               {
                                   return commandListenerContext.listener != nullptr
                                          && commandListenerContext.listener->getCommandId()
                                                 == commandId;
                               });
       mCommandListenerContexts.end() != ite)
    {
        listenerContext = *ite;
    }

    return listenerContext;
}

bool NrfCommunication::processRxDataParseDataByte(uint8_t data)
{
    bool bRet = true;

    if(m_rxDataVector.available() != 0)
    {
        m_rxDataVector.push_back(data);
    }
    else
    {
        bRet = false;
    }

    ++mRxCurrentDataOffset;

    if(mRxCurrentFrameOffset == (RX_PROTOCOL_FRAME_LEN_IDX + mRxCurrentFrameLength) - 1)
    {
        mFrameRxParseState = RXPARSESTATE_EOF;
    }

    return bRet;
}

bool NrfCommunication::processRxDataParseByte(uint8_t data)
{
    bool bRet = false;

    switch(mFrameRxParseState)
    {
    case RXPARSESTATE_WAITSOF:
        bRet = processRxDataParseSOFByte(data);
        break;
    case RXPARSESTATE_HEADER:
        bRet = processRxDataParseHeaderByte(data);
        break;
    case RXPARSESTATE_DATA:
        bRet = processRxDataParseDataByte(data);
        break;
    case RXPARSESTATE_EOF:
        bRet = processRxDataParseEOFByte(data);
        break;
    default:
        break;
    }

    return bRet;
}

bool NrfCommunication::processRxDataParseEOFByte(uint8_t data)
{
    bool bRet = true;

    if((mRxCurrentFrameOffset == (RX_PROTOCOL_FRAME_LEN_IDX + mRxCurrentFrameLength))
       && (data == RX_PROTOCOL_EOF))
    {
        if(mRxCurrentDataOffset == mRxCurrentDataLength)
        {
            notifyListener();

            resetRxProtocolDataParsing();
        }
        else
        {
            resetRxProtocolFrameParsing();
        }
    }
    else
    {
        bRet = false;
    }

    return bRet;
}

void NrfCommunication::resetRxProtocolDataParsing()
{
    resetRxProtocolFrameParsing();
    mRxCurrentDataLength = 0;
    mRxCurrentCommandId = BLESPICommands::CommandId::INVALID_CMD;
    mRxCurrentDataOffset = 0;
    m_rxDataVector.clear();
}

void NrfCommunication::resetRxProtocolFrameParsing()
{
    mRxCurrentFrameLength = 0;
    mRxCurrentFrameOffset = 0;
    mFrameRxParseState = RXPARSESTATE_WAITSOF;

    mRxCurrentDataLengthFromHeader = 0;
    mRxCurrentDataOffsetFromHeader = 0;
    mRxCurrentCommandIdFromHeader = BLESPICommands::CommandId::INVALID_CMD;
}

bool NrfCommunication::processRxDataParseSOFByte(uint8_t data)
{
    bool bRet = true;

    if((mRxCurrentFrameOffset == RX_PROTOCOL_SOF_IDX) && (data == RX_PROTOCOL_SOF))
    {
        mFrameRxParseState = RXPARSESTATE_HEADER;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}

void NrfCommunication::registerCommandListener(AbstractBleCommandListener& commandListener)
{
    registerListener(commandListener, nullptr);
}

void NrfCommunication::unregisterListener(AbstractBleCommandListener& commandListener)
{
    AutoLock autoLock(mListenersMutex);

    auto ite = std::find_if(mCommandListenerContexts.begin(),
                            mCommandListenerContexts.end(),
                            [&commandListener](auto& listenerContext)
                            { return listenerContext.listener == &commandListener; });

    if(mCommandListenerContexts.end() != ite)
    {
        if(ite->timer)
        {
            ite->timer->stop();
            releaseRequestTimerFromPool(ite->timer);
        }
        mCommandListenerContexts.erase(ite);
    }
}

void NrfCommunication::notification(const BleRequestTimerEvent& event)
{
    BleTxRequest* request = &event.request.get();
    // Run the timeout routine in a workqueue in order to avoid timer task stack overflow
    // This also makes possible to delete the timer in this routine (this cannot be done here as we are inside the timer notification)
    ASSERT(mWorkQueue.submitData(mRequestTimeoutWork,
                                 {reinterpret_cast<uint8_t*>(&request), sizeof(request)}));
}

void NrfCommunication::parseRxData(const uint8_t* buffer, size_t bufferLen)
{
    for(size_t i = 0; i < bufferLen; i++)
    {
        bool bParseSuccess = processRxDataParseByte(buffer[i]);
        if(mFrameRxParseState != RXPARSESTATE_WAITSOF)
        {
            if(bParseSuccess)
            {
                mRxCurrentFrameOffset++;
            }
            else
            {
                resetRxProtocolDataParsing();
            }
        }
    }
}

void NrfCommunication::processRxData(const uint8_t* buffer, size_t bufferLen)
{
    if(bufferLen > 0)
    {
        size_t packetDataSize = buffer[0];
        --bufferLen;
        ++buffer;
        packetDataSize = std::min(packetDataSize, bufferLen);
        parseRxData(buffer, packetDataSize);
    }
}

void NrfCommunication::vNrfCommunicationTask(void* pvParameters)
{
    auto bleHID = reinterpret_cast<NrfCommunication*>(pvParameters);
    Message data;
    auto delay = DelayMs(DelayMs::INFINITE_DELAY);
    for(;;)
    {
        if(pdPASS == xQueueReceive(bleHID->mQueue, &data, delayToSystemTicks(delay)))
        {
            switch(data.msgId)
            {
            case MessageId::INIT:
                bleHID->execInit();
                delay = NO_TRANSFER_IN_PROGRESS_POLL_DELAY_MS;
                break;
            case MessageId::POWEROFF:
                bleHID->execPowerOff();
                break;
            case MessageId::FORCEDFU:
                bleHID->execForceDfu();
                break;
            case MessageId::DATATOSSEND:
                bleHID->spiReceive();
                break;
            case MessageId::SEND_COMMAND:
                bleHID->execSendCommand(data.sendCommandArg.packet);
                break;
            default:
                break;
            }
        }
        else
        {
            bleHID->spiReceive();
        }

        delay = bleHID->mCommStrategy.get().isDataToSend()
                    ? TRANSFER_IN_PROGRESS_POLL_DELAY_MS
                    : bleHID->mCommStrategy.get().getPollDelay();
    }
}

void NrfCommunication::execInit()
{
    TRACE_FUNC_ENTRY();

    mCommStrategy.get().init();

    HAL_GPIO_WritePin(CPU_BLE__RST_GPIO_Port, CPU_BLE__RST_Pin, GPIO_PIN_RESET);
    osDelay(10);

    mCommStrategy.get().resetHostReadyState();
    HAL_GPIO_WritePin(CPU_BLE__RST_GPIO_Port, CPU_BLE__RST_Pin, GPIO_PIN_SET);

    TRACE_FUNC_EXIT();
}

void NrfCommunication::spiProtocolSetHeader(
    BLESPICommands::CommandId command,
    size_t commandLength,
    etl::vector<uint8_t, BLESPICommands::MAX_COMMAND_LENGTH>& dataToSend)
{
    ASSERT(commandLength < UINT16_MAX);
    dataToSend.push_back((uint8_t) command);
    dataToSend.push_back(reinterpret_cast<uint8_t*>(&commandLength)[1]);
    dataToSend.push_back(reinterpret_cast<uint8_t*>(&commandLength)[0]);
}

void NrfCommunication::spiProtocolSetParameters(
    const BleCmdPacket::ParameterList& parameters,
    etl::vector<uint8_t, BLESPICommands::MAX_COMMAND_LENGTH>& dataToSend)
{
    dataToSend.insert(dataToSend.end(), parameters.begin(), parameters.end());
}

void NrfCommunication::spiProtocolSendCommand(BLESPICommands::CommandId command,
                                              const BleCmdPacket::ParameterList& parameters)
{
    uint32_t commandLength = parameters.size();

    etl::vector<uint8_t, BLESPICommands::MAX_COMMAND_LENGTH> dataToSend;

    spiProtocolSetHeader(command, commandLength, dataToSend);

    spiProtocolSetParameters(parameters, dataToSend);

    spiTransmitReceive(dataToSend.data(), dataToSend.size());
}

BLEDefs::ErrCode NrfCommunication::powerOff()
{
    Message message = {};
    message.msgId = MessageId::POWEROFF;

    mPowerOffDoneSignal.clear();

    BLEDefs::ErrCode ret = xQueueSendToBack(mQueue, (void*) &message, MSG_TIMEOUT)
                               ? BLEDefs::ErrCode::SUCCESS
                               : BLEDefs::ErrCode::BUSY;

    if(ret == BLEDefs::ErrCode::SUCCESS)
    {
        mPowerOffDoneSignal.wait(DelayMs(DelayMs::INFINITE_DELAY));
    }

    return ret;
}

void NrfCommunication::execPowerOff()
{
    HAL_GPIO_WritePin(CPU_BLE__RST_GPIO_Port, CPU_BLE__RST_Pin, GPIO_PIN_RESET);

    mPowerOffDoneSignal.set();
}

void NrfCommunication::initDummyTxBuffer()
{
    memset(mDummyTxBuffer, DEFAULT_TX_VALUE, MAX_TRANSFER_LEN);
}

BLEDefs::ErrCode NrfCommunication::forceDfu()
{
    BLEDefs::ErrCode ret = BLEDefs::ErrCode::SUCCESS;

    Message message = {};
    message.msgId = MessageId::FORCEDFU;
    if(pdPASS != xQueueSendToBack(mQueue, (void*) &message, MSG_TIMEOUT))
    {
        ret = BLEDefs::ErrCode::BUSY;
    }

    return ret;
}

BLEDefs::ErrCode NrfCommunication::execForceDfu()
{
    BLEDefs::ErrCode ret = BLEDefs::ErrCode::SUCCESS;
    HAL_GPIO_WritePin(CPU_BLE__RST_GPIO_Port, CPU_BLE__RST_Pin, GPIO_PIN_RESET);
    initGpioForceDfu();
    setForceDfuGpio();
    clearDfuAck();

    osDelay(10);
    HAL_GPIO_WritePin(CPU_BLE__RST_GPIO_Port, CPU_BLE__RST_Pin, GPIO_PIN_SET);

    if(!waitDfuAck())
    {
        ret = BLEDefs::ErrCode::DEVICE_NOT_READY;
        LOG_ERR(MODULE,
                "No DFU ack " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                    static_cast<uint16_t>(ret)));
    }

    deinitGpioForceDfu();

    return ret;
}

void NrfCommunication::bleDataToSendAvailableIsr()
{
    Message message = {.msgId = MessageId::DATATOSSEND};

    BaseType_t xHigherPriorityTaskWoken;
    xQueueSendToBackFromISR(mQueue, &message, &xHigherPriorityTaskWoken);

    if(xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void NrfCommunication::changeCommStrategy(IBLECommStrategy& newCommStrategy)
{
    mCommStrategy.get().deinit();
    mCommStrategy = std::ref<IBLECommStrategy>(newCommStrategy);
    mCommStrategy.get().init();
}

void NrfCommunication::initGpioForceDfu()
{
    mISPI.deinit();

    GPIOOutConfig gpioOutConfig = {IGPIO_OUTTYPE_PUSHPULL, IGPIO_NO_PULL};

    mGPIOController.configureOutput(mBLEIO.mosi, gpioOutConfig);

    GPIOInConfig gpioInConfig;

    std::function<void()> ackDfuCallback = std::bind(&NrfCommunication::ackDfuCallback, this);

    gpioInConfig.mode = IGPIO_INMODE_EXTINT;
    gpioInConfig.pull = IGPIO_PULL_DOWN;
    gpioInConfig.itMode = IGPIO_ITMODE_RISING;
    gpioInConfig.itCallback = ackDfuCallback;

    mGPIOController.configureInput(mBLEIO.miso, gpioInConfig);
}

void NrfCommunication::deinitGpioForceDfu()
{
    mGPIOController.deinit(mBLEIO.mosi);
    mGPIOController.deinit(mBLEIO.miso);

    mISPI.init(mSPIConfig);
}

void NrfCommunication::setForceDfuGpio()
{
    mGPIOController.set(mBLEIO.mosi);
}

void NrfCommunication::clearDfuAck()
{
    mAckDfuSignal.clear();
}

bool NrfCommunication::waitDfuAck()
{
    return mAckDfuSignal.wait(DelayMs(WAIT_ACK_DFU_DELAY_MS));
}

void NrfCommunication::ackDfuCallback()
{
    mAckDfuSignal.setFromISR();
}

BleCmdPacket* NrfCommunication::allocatePacketFromPool()
{
    AutoLock autoLock(mCmdPacketPoolMutex);
    return mCmdPacketPool.create();
}

void NrfCommunication::releasePacketToPool(const BleCmdPacket* packet)
{
    AutoLock autoLock(mCmdPacketPoolMutex);
    mCmdPacketPool.destroy(packet);
}

BleRequestTimer* NrfCommunication::allocateRequestTimerFromPool(BleTxRequest& request)
{
    AutoLock autoLock(mRequestTimerPoolMutex);
    return mRequestTimerPool.create(request);
}

void NrfCommunication::releaseRequestTimerFromPool(const BleRequestTimer* timer)
{
    AutoLock autoLock(mRequestTimerPoolMutex);
    mRequestTimerPool.destroy(timer);
}

void NrfCommunication::execSendCommand(const BleCmdPacket* packet)
{
    spiProtocolSendCommand(packet->getId(), packet->getParameters());

    releasePacketToPool(packet);
}

void NrfCommunication::registerRequestListener(AbstractBleCommandListener& requestListener,
                                               BleRequestTimer* timer)
{
    registerListener(requestListener, timer);
}

void NrfCommunication::registerListener(AbstractBleCommandListener& listener, BleRequestTimer* timer)
{
    AutoLock autoLock(mListenersMutex);

    auto ite = std::find_if(mCommandListenerContexts.begin(),
                            mCommandListenerContexts.end(),
                            [&listener](auto& listenerContext)
                            { return listenerContext.listener == &listener; });
    if(mCommandListenerContexts.end() == ite)
    {
        mCommandListenerContexts.push_back(CommandListenerContext{&listener, timer});
    }
}

void NrfCommunication::notifyListener()
{
    CommandListenerContext commandListenerContext = findBleCommandListenerContext(
        mRxCurrentCommandId);

    if(commandListenerContext.listener != nullptr)
    {
        commandListenerContext.listener->receivedData(m_rxDataVector.data(), m_rxDataVector.size());
        if(commandListenerContext.timer != nullptr)
        {
            unregisterListener(*commandListenerContext.listener);
        }
    }
}

void NrfCommunication::onRequestTimeout(etl::array_view<uint8_t> data)
{
    BleTxRequest& request = **reinterpret_cast<BleTxRequest**>(data.data());

    LOG_ERR(MODULE,
            "Request " << static_cast<uint8_t>(request.getId()) << " timeout error"
                       << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                             static_cast<uint16_t>(
                                                                 BLEDefs::ErrCode::RESP_TIMEOUT)));

    unregisterListener(request);
    request.onTimeout();
}