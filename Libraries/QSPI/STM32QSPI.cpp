/*
 * STM32QuadFlash.cpp
 *
 *  Created on: 2 déc. 2021
 *      Author: thomas
 */

#include <STM32QSPI.hpp>
#include <utility>
#include <functional>
#include <array>
#include <optional>
#include "utils.hpp"
#include "CommonFlashInterface.hpp"

/* Handle only one QSPI instance for now */
std::pair<QSPI_HandleTypeDef*, std::optional<std::reference_wrapper<ISTM32QuadSPIIRQListener>>>
    mIRQListener = std::make_pair(nullptr, std::nullopt);

/**
* @brief This function handles QUADSPI global interrupt.
*/
extern "C" void QUADSPI_IRQHandler(void)
{
    /* USER CODE BEGIN QUADSPI_IRQn 0 */

    /* USER CODE END QUADSPI_IRQn 0 */
    if(mIRQListener.first != NULL)
    {
        HAL_QSPI_IRQHandler(mIRQListener.first);
    }
    /* USER CODE BEGIN QUADSPI_IRQn 1 */

    /* USER CODE END QUADSPI_IRQn 1 */
}

/**
  * @brief  This function handles QUADSPI DMA interrupt request.
  * @param  None
  * @retval None
  */
extern "C" void DMA2_Channel7_IRQHandler(void)
{
    if(mIRQListener.first != NULL)
    {
        HAL_DMA_IRQHandler(mIRQListener.first->hdma);
    }
}

/**
 * @brief  Command completed callbacks.
 * @param  hqspi: QSPI handle
 * @retval None
 */
extern "C" void HAL_QSPI_CmdCpltCallback(QSPI_HandleTypeDef* hqspi)
{
    STM32QSPI::getIRQListener(hqspi).getCmdCpltSignal().setFromISR();
}

/**
 * @brief  Rx Transfer completed callbacks.
 * @param  hqspi: QSPI handle
 * @retval None
 */
extern "C" void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef* hqspi)
{
    STM32QSPI::getIRQListener(hqspi).getRxCpltSignal().setFromISR();
}

/**
 * @brief  Tx Transfer completed callbacks.
 * @param  hqspi: QSPI handle
 * @retval None
 */
extern "C" void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef* hqspi)
{
    STM32QSPI::getIRQListener(hqspi).getTxCpltSignal().setFromISR();
}

/**
  * @brief  Status Match callbacks
  * @param  hqspi: QSPI handle
  * @retval None
  */
extern "C" void HAL_QSPI_StatusMatchCallback(QSPI_HandleTypeDef* hqspi)
{
    STM32QSPI::getIRQListener(hqspi).getStatusMatchSignal().setFromISR();
}

void STM32QSPI::registerIRQListener(QSPI_HandleTypeDef* hqspi, STM32QSPI& qspi)
{
    ASSERT(mIRQListener.first == nullptr || mIRQListener.first == hqspi);

    mIRQListener = std::make_pair(hqspi, std::reference_wrapper<STM32QSPI>(qspi));
}

ISTM32QuadSPIIRQListener& STM32QSPI::getIRQListener(QSPI_HandleTypeDef* hqspi)
{
    ASSERT(mIRQListener.first == hqspi);

    return mIRQListener.second.value().get();
}

STM32QSPI::STM32QSPI(QUADSPI_TypeDef* qspiInstance)
    : m_qspiInstance(qspiInstance)
{
}

IQSPIErrCode STM32QSPI::init(size_t size)
{
    ASSERT(m_qspiInstance != nullptr);

    m_hQSPI.Instance = m_qspiInstance;
    HAL_QSPI_DeInit(&m_hQSPI);

    m_hQSPI.Init.ClockPrescaler = 1;
    m_hQSPI.Init.FifoThreshold = 4;
    m_hQSPI.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
    m_hQSPI.Init.FlashSize = numberToPowerOfTwo(size) - 1;
    m_hQSPI.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE;
    m_hQSPI.Init.ClockMode = QSPI_CLOCK_MODE_0;
    m_hQSPI.Init.FlashID = QSPI_FLASH_ID_1;
    m_hQSPI.Init.DualFlash = QSPI_DUALFLASH_DISABLE;

    if(HAL_QSPI_Init(&m_hQSPI) != HAL_OK)
    {
        return IQSPIErrCode::IQSPI_ERROR_UNKNOWN;
    }

    registerIRQListener(&m_hQSPI, *this);

    return IQSPIErrCode::IQSPI_ERROR_SUCCESS;
}

IQSPIErrCode STM32QSPI::deinit()
{
    return (HAL_QSPI_DeInit(&m_hQSPI) != HAL_OK ? IQSPIErrCode::IQSPI_ERROR_UNKNOWN
                                                : IQSPIErrCode::IQSPI_ERROR_SUCCESS);
}

void STM32QSPI::initCommandStructure(QSPI_CommandTypeDef* sCommand)
{
    sCommand->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand->AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand->DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
}

uint32_t STM32QSPI::dataNbLinesToHAL(uint8_t nbLines)
{
    uint32_t halNbLines = QSPI_DATA_NONE;

    switch(nbLines)
    {
    case 0:
        break;
    case 1:
        halNbLines = QSPI_DATA_1_LINE;
        break;
    case 2:
        halNbLines = QSPI_DATA_2_LINES;
        break;
    case 4:
        halNbLines = QSPI_DATA_4_LINES;
        break;
    default:
        ASSERT(false);
        break;
    }

    return halNbLines;
}

uint32_t STM32QSPI::addrModeToHal(SFlashCommunicationInterfaceAddrMode addrMode)
{
    uint32_t addrSize = QSPI_ADDRESS_24_BITS;

    switch(addrMode)
    {
    case SFlashCommunicationInterfaceAddrMode::ADDR_MODE_3_BYTES:
        addrSize = QSPI_ADDRESS_24_BITS;
        break;
    case SFlashCommunicationInterfaceAddrMode::ADDR_MODE_4_BYTES:
        addrSize = QSPI_ADDRESS_32_BITS;
        break;
    default:
        break;
    }

    return addrSize;
}

IQSPIErrCode STM32QSPI::transmit(FlashCommand& command)
{
    IQSPIErrCode err = IQSPIErrCode::IQSPI_ERROR_UNKNOWN;

    QSPI_CommandTypeDef sCommand = {0};

    initCommandStructure(&sCommand);

    sCommand.Instruction = command.command;
    sCommand.AddressMode = (command.addrMode == SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE)
                               ? QSPI_ADDRESS_NONE
                               : QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize = addrModeToHal(command.addrMode);
    sCommand.Address = command.address;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if(sendQSPICommand(&sCommand))
    {
        err = IQSPIErrCode::IQSPI_ERROR_SUCCESS;
    }

    return err;
}

IQSPIErrCode STM32QSPI::transmit(FlashCommand& command, uint8_t* txData, size_t size, bool bDMA)
{
    IQSPIErrCode err = IQSPIErrCode::IQSPI_ERROR_UNKNOWN;

    QSPI_CommandTypeDef sCommand = {0};

    initCommandStructure(&sCommand);

    sCommand.Instruction = command.command;
    sCommand.AddressMode = (command.addrMode == SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE)
                               ? QSPI_ADDRESS_NONE
                               : QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize = addrModeToHal(command.addrMode);
    sCommand.Address = command.address;
    sCommand.DataMode = dataNbLinesToHAL(command.dataNbLines);
    sCommand.DummyCycles = 0;
    sCommand.NbData = size;

    if(sendQSPICommand(&sCommand))
    {
        err = IQSPIErrCode::IQSPI_ERROR_SUCCESS;
    }

    if(err == IQSPIErrCode::IQSPI_ERROR_SUCCESS && bDMA)
    {
        err = transmitQSPIDataDMA(txData) ? IQSPIErrCode::IQSPI_ERROR_SUCCESS
                                          : IQSPIErrCode::IQSPI_ERROR_UNKNOWN;
    }

    if(err == IQSPIErrCode::IQSPI_ERROR_SUCCESS && !bDMA)
    {
        err = transmitQSPIData(txData) ? IQSPIErrCode::IQSPI_ERROR_SUCCESS
                                       : IQSPIErrCode::IQSPI_ERROR_UNKNOWN;
    }

    return err;
}

IQSPIErrCode STM32QSPI::receive(FlashCommand& command, uint8_t* rxData, size_t size, bool bDMA)
{
    IQSPIErrCode err = IQSPIErrCode::IQSPI_ERROR_UNKNOWN;

    QSPI_CommandTypeDef sCommand = {0};

    initCommandStructure(&sCommand);

    sCommand.Instruction = command.command;
    sCommand.AddressMode = (command.addrMode == SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE)
                               ? QSPI_ADDRESS_NONE
                               : QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize = addrModeToHal(command.addrMode);
    sCommand.Address = command.address;
    sCommand.DataMode = dataNbLinesToHAL(command.dataNbLines);
    sCommand.DummyCycles = command.dummy;
    sCommand.NbData = size;

    if(sendQSPICommand(&sCommand))
    {
        err = IQSPIErrCode::IQSPI_ERROR_SUCCESS;
    }

    if(err == IQSPIErrCode::IQSPI_ERROR_SUCCESS && bDMA)
    {
        err = receiveQSPIDataDMA(rxData) ? IQSPIErrCode::IQSPI_ERROR_SUCCESS
                                         : IQSPIErrCode::IQSPI_ERROR_UNKNOWN;
    }

    if(err == IQSPIErrCode::IQSPI_ERROR_SUCCESS && !bDMA)
    {
        err = receiveQSPIData(rxData) ? IQSPIErrCode::IQSPI_ERROR_SUCCESS
                                      : IQSPIErrCode::IQSPI_ERROR_UNKNOWN;
    }

    return err;
}

IQSPIErrCode STM32QSPI::transmitAndWaitValue(FlashCommand& command, FlashWaitValue value)
{
    ASSERT(value.matchOperator == SFlashCommunicationInterfaceMatchOperator::MATCH_OPERATOR_AND);

    IQSPIErrCode err = IQSPIErrCode::IQSPI_ERROR_UNKNOWN;

    QSPI_CommandTypeDef sCommand = {0};
    QSPI_AutoPollingTypeDef sConfig = {0};

    initCommandStructure(&sCommand);

    sCommand.Instruction = command.command;
    sCommand.AddressMode = (command.addrMode == SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE)
                               ? QSPI_ADDRESS_NONE
                               : QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize = addrModeToHal(command.addrMode);
    sCommand.Address = command.address;
    sCommand.DataMode = dataNbLinesToHAL(command.dataNbLines);
    sCommand.DummyCycles = 0;
    sCommand.NbData = value.size;

    if(command.command != 0 && !sendQSPICommand(&sCommand))
    {
        return err;
    }

    sConfig.Match = value.match;
    sConfig.Mask = value.mask;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = value.size;
    sConfig.Interval = value.periodMs;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    sCommand.Instruction = value.waitCommand;
    sCommand.DataMode = dataNbLinesToHAL(value.waitCommandDataNbLines);


    if(autoPollQSPI_IT(&sCommand, &sConfig))
    {
        err = IQSPIErrCode::IQSPI_ERROR_SUCCESS;
    }

    return err;
}

bool STM32QSPI::sendQSPICommand_IT(QSPI_CommandTypeDef* cmd)
{
    cmdCplt.clear();

    if(HAL_QSPI_Command_IT(&m_hQSPI, cmd) != HAL_OK)
    {
        return false;
    }

    if(!cmdCplt.wait(DelayMs(QSPI_TIMEOUT_MS)))
    {
        return false;
    }

    return true;
}

bool STM32QSPI::sendQSPICommand(QSPI_CommandTypeDef* cmd)
{
    if(HAL_QSPI_Command(&m_hQSPI, cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return false;
    }

    return true;
}


bool STM32QSPI::transmitQSPIData_IT(uint8_t* pData)
{
    txCplt.clear();

    if(HAL_QSPI_Transmit_IT(&m_hQSPI, pData) != HAL_OK)
    {
        return false;
    }

    if(!txCplt.wait(DelayMs(QSPI_TIMEOUT_MS)))
    {
        return false;
    }

    return true;
}

bool STM32QSPI::transmitQSPIData(uint8_t* pData)
{
    if(HAL_QSPI_Transmit(&m_hQSPI, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return false;
    }

    return true;
}

bool STM32QSPI::transmitQSPIDataDMA(uint8_t* pData)
{
    txCplt.clear();

    if(HAL_QSPI_Transmit_DMA(&m_hQSPI, pData) != HAL_OK)
    {
        return false;
    }

    if(!txCplt.wait(DelayMs(QSPI_TIMEOUT_MS)))
    {
        return false;
    }

    return true;
}

bool STM32QSPI::receiveQSPIData(uint8_t* pData)
{
    if(HAL_QSPI_Receive(&m_hQSPI, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return false;
    }

    return true;
}

bool STM32QSPI::receiveQSPIDataDMA(uint8_t* pData)
{
    rxCplt.clear();

    if(HAL_QSPI_Receive_DMA(&m_hQSPI, pData) != HAL_OK)
    {
        return false;
    }

    if(!rxCplt.wait(DelayMs(QSPI_TIMEOUT_MS)))
    {
        return false;
    }

    return true;
}

bool STM32QSPI::autoPollQSPI_IT(QSPI_CommandTypeDef* cmd, QSPI_AutoPollingTypeDef* cfg)
{
    static HAL_StatusTypeDef status = HAL_OK;
    statusMatch.clear();

    status = HAL_QSPI_AutoPolling_IT(&m_hQSPI, cmd, cfg);
    if(status != HAL_OK)
    {
        return false;
    }

    if(!statusMatch.wait(DelayMs(QSPI_TIMEOUT_MS)))
    {
        return false;
    }

    return true;
}

bool STM32QSPI::autoPollQSPI(QSPI_CommandTypeDef* cmd, QSPI_AutoPollingTypeDef* cfg)
{
    if(HAL_QSPI_AutoPolling(&m_hQSPI, cmd, cfg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return false;
    }

    return true;
}

Signal& STM32QSPI::getCmdCpltSignal()
{
    return cmdCplt;
}

Signal& STM32QSPI::getTxCpltSignal()
{
    return txCplt;
}

Signal& STM32QSPI::getRxCpltSignal()
{
    return rxCplt;
}

Signal& STM32QSPI::getStatusMatchSignal()
{
    return statusMatch;
}

QSPI_HandleTypeDef& STM32QSPI::getQSPIHandle()
{
    return m_hQSPI;
}
