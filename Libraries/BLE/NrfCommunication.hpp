/*
 * NrfCommunication.h
 *
 *  Created on: May 11, 2017
 *      Author: Labo
 */

#ifndef BLE_BLECOMMANDS_H_
#define BLE_BLECOMMANDS_H_

#include "IBleCommunication.hpp"
#include "iSPI.h"
#include "SingleChipSelector.h"
#include "IUsbActivable.h"
#include "IPowerManageable.h"
#include "usbPowerState.h"
#include "Signal.hpp"
#include "ApplicationConfig.h"
#include <set>
#include "BLESPICommands.hpp"
#include "IConnectivityListener.hpp"
#include <variant>
#include "ITBLECommStrategy.hpp"
#include "BleCmdPacket.hpp"
#include "BleTxCmd.hpp"
#include "etl/pool.h"
#include "etl/optional.h"
#include "Mutex.hpp"
#include "BleRequestTimer.hpp"
#include "IWorkQueue.hpp"
#include "Module.hpp"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

using std::vector;

#define MAX_TRANSFER_LEN ((size_t) 255)

class NrfCommunication : public IBleCommunication,
                         public Module<Identification::ModuleId::BLE>,
                         private IITBLECommStrategyListener,
                         private BleRequestTimerObserver

{
public:
    struct BLEIO
    {
        GPIOPin csN;
        GPIOPin hrdy;
        GPIOPin dts;
        GPIOPin miso;
        GPIOPin mosi;

        BLEIO(GPIOPin _csN, GPIOPin _hrdy, GPIOPin _dts, GPIOPin _miso, GPIOPin _mosi)
            : csN(_csN)
            , hrdy(_hrdy)
            , dts(_dts)
            , miso(_miso)
            , mosi(_mosi)
        {
        }
    };

    NrfCommunication(iSPI& iSPI,
                     IGPIOController& gpioController,
                     BLEIO& bleIO,
                     IWorkQueue& workQueue);
    virtual ~NrfCommunication() = default;
    BLEDefs::ErrCode init() final;

    BLEDefs::ErrCode sendCommand(BleTxCmd& cmd) final;
    BLEDefs::ErrCode sendRequest(BleTxRequest& cmd) final;
    BLEDefs::ErrCode forceDfu() final;

    BLEDefs::ErrCode powerOff() final;

    void registerCommandListener(AbstractBleCommandListener& commandListener) override;
    void unregisterListener(AbstractBleCommandListener& commandListener) override;

    void notification(const BleRequestTimerEvent& event) final;

private:
    static constexpr UBaseType_t BLE_TASK_PRIORITY = 2;
    static constexpr configSTACK_DEPTH_TYPE BLE_TASK_STACK_SIZE = 2048;
    static constexpr TickType_t MSG_TIMEOUT = 10;
    static constexpr TickType_t RESP_TIMEOUT = 1000;
    static constexpr int32_t BOOT_DELAY_MS = 3000;
    static constexpr uint8_t DEFAULT_TX_VALUE = 0xFF;
    static constexpr int32_t TRANSFER_IN_PROGRESS_POLL_DELAY_MS = 4;
    static constexpr int32_t NO_TRANSFER_IN_PROGRESS_POLL_DELAY_MS = 100;
    static constexpr int32_t WAIT_HOST_READY_DELAY_MS = 200;
    static constexpr int32_t WAIT_ACK_DFU_DELAY_MS = 800;
    static constexpr size_t CMD_PACKET_POOL_SIZE = 5;
    static constexpr size_t REQUEST_TIMER_POOL_SIZE = 2;
    static constexpr size_t MAX_RX_DATA_SIZE = 1024;

    enum class MessageId
    {
        INIT,
        POWEROFF,
        FORCEDFU,
        DATATOSSEND,
        SEND_COMMAND
    };

    struct SendCommandArg
    {
        BleCmdPacket* packet;
    };

    struct Message
    {
        MessageId msgId;
        union
        {
            SendCommandArg sendCommandArg;
        };
    };

    struct SPIParameter
    {
        uint16_t length;
        const uint8_t* pData;
    };

    enum RxParseState
    {
        RXPARSESTATE_WAITSOF = 0,
        RXPARSESTATE_HEADER,
        RXPARSESTATE_DATA,
        RXPARSESTATE_EOF
    };

    static SPIConfig gBleCommunicationSPIConfig;

    SingleChipSelector mChipSelector;
    iSPI& mISPI;
    SPIConfig mSPIConfig{gBleCommunicationSPIConfig};

    RxParseState mFrameRxParseState{RXPARSESTATE_WAITSOF};
    uint8_t mRxCurrentFrameLength{0};
    uint8_t mRxCurrentFrameOffset{0};

    uint16_t mRxCurrentDataLength{0};
    uint16_t mRxCurrentDataLengthFromHeader{0}; // Value read from frame header
    BLESPICommands::CommandId mRxCurrentCommandId{BLESPICommands::CommandId::INVALID_CMD};
    BLESPICommands::CommandId mRxCurrentCommandIdFromHeader{
        BLESPICommands::CommandId::INVALID_CMD}; // Value read from frame header
    uint16_t mRxCurrentDataOffset{0};
    uint16_t mRxCurrentDataOffsetFromHeader{0}; // Value read from frame header

    struct CommandListenerContext
    {
        AbstractBleCommandListener* listener;
        BleRequestTimer* timer;
    };
    Mutex mListenersMutex;
    std::vector<CommandListenerContext> mCommandListenerContexts;

    etl::vector<uint8_t, MAX_RX_DATA_SIZE> m_rxDataVector;

    Signal mDeviceCountSignal;
    Signal mDeviceInfoSignal;
    Signal mProtocolVersionSignal;

    QueueHandle_t mQueue{nullptr};
    TaskHandle_t mHandle{nullptr};

    uint8_t mRxBuffer[MAX_TRANSFER_LEN];
    uint8_t mDummyTxBuffer[MAX_TRANSFER_LEN];

    Signal mPowerOffDoneSignal;

    IGPIOController& mGPIOController;
    BLEIO& mBLEIO;
    ITBLECommStrategy mITCommStrategy;
    std::reference_wrapper<IBLECommStrategy> mCommStrategy;

    Signal mAckDfuSignal;


    Mutex mCmdPacketPoolMutex;
    etl::pool<BleCmdPacket, CMD_PACKET_POOL_SIZE> mCmdPacketPool;

    Mutex mRequestTimerPoolMutex;
    etl::pool<BleRequestTimer, REQUEST_TIMER_POOL_SIZE> mRequestTimerPool;

    IWorkQueue& mWorkQueue;

    Work mRequestTimeoutWork;

    static void vNrfCommunicationTask(void* pvParameters);

    void execInit();
    BLEDefs::ErrCode execForceDfu();

    void execPowerOff();

    void execSendCommand(const BleCmdPacket* packet);

    BLEDefs::ErrCode spiTransmitReceive(const uint8_t* data, size_t length);
    BLEDefs::ErrCode spiTransmitReceiveSingle(const uint8_t* data, size_t length);
    BLEDefs::ErrCode spiReceive();

    void spiProtocolSendCommand(BLESPICommands::CommandId command,
                                const BleCmdPacket::ParameterList& parameters);
    void spiProtocolSetHeader(BLESPICommands::CommandId command,
                              size_t commandLength,
                              etl::vector<uint8_t, BLESPICommands::MAX_COMMAND_LENGTH>& dataToSend);
    void spiProtocolSetParameters(
        const BleCmdPacket::ParameterList& parameters,
        etl::vector<uint8_t, BLESPICommands::MAX_COMMAND_LENGTH>& dataToSend);

    CommandListenerContext findBleCommandListenerContext(BLESPICommands::CommandId commandId);
    bool isRxDataTransferInProgress() const;
    bool processRxDataParseSOFByte(uint8_t data);
    bool processRxDataParseHeaderByte(uint8_t data);
    bool processRxDataParseDataByte(uint8_t data);
    bool processRxDataParseEOFByte(uint8_t data);
    bool processRxDataParseByte(uint8_t data);
    void resetRxProtocolDataParsing();
    void resetRxProtocolFrameParsing();
    void processRxData(const uint8_t* buffer, size_t bufferLen);
    void parseRxData(const uint8_t* buffer, size_t bufferLen);
    bool isCommandWithResponse(BLESPICommands::CommandId command);
    void initDummyTxBuffer();
    void incResponseRequest();
    void decResponseRequest();

    void bleDataToSendAvailableIsr() override;
    void changeCommStrategy(IBLECommStrategy& newCommStrategy);

    void initGpioForceDfu();
    void deinitGpioForceDfu();
    void setForceDfuGpio();
    void clearDfuAck();
    bool waitDfuAck();
    void ackDfuCallback();

    BleCmdPacket* allocatePacketFromPool();
    void releasePacketToPool(const BleCmdPacket* packet);

    BleRequestTimer* allocateRequestTimerFromPool(BleTxRequest& request);
    void releaseRequestTimerFromPool(const BleRequestTimer* timer);

    void registerRequestListener(AbstractBleCommandListener& requestListener,
                                 BleRequestTimer* timer);
    void registerListener(AbstractBleCommandListener& listener, BleRequestTimer* timer);
    void notifyListener();

    void onRequestTimeout(etl::array_view<uint8_t> data);
};

#endif /* BLE_BLECOMMANDS_H_ */
