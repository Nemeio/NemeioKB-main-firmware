/*
 * STM32QuadFlash.hpp
 *
 *  Created on: 2 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_STM32QSPI_HPP_
#define SFLASH_STM32QSPI_HPP_

#include "stm32l4xx_hal.h"
#include "Signal.hpp"
#include "ISTM32QuadSPIIRQListener.hpp"
#include "iQSPI.hpp"

class STM32QSPI : public iQSPI, public ISTM32QuadSPIIRQListener
{
public:
    explicit STM32QSPI(QUADSPI_TypeDef* qspiInstance);
    virtual ~STM32QSPI() = default;

    IQSPIErrCode init(size_t size) final;
    IQSPIErrCode deinit() final;

    IQSPIErrCode transmit(FlashCommand& command) final;
    IQSPIErrCode transmit(FlashCommand& command,
                          uint8_t* txData,
                          size_t size,
                          bool bDMA = false) final;
    IQSPIErrCode receive(FlashCommand& command,
                         uint8_t* rxData,
                         size_t size,
                         bool bDMA = false) final;

    IQSPIErrCode transmitAndWaitValue(FlashCommand& command, FlashWaitValue value) final;

    Signal& getCmdCpltSignal() final;
    Signal& getTxCpltSignal() final;
    Signal& getRxCpltSignal() final;
    Signal& getStatusMatchSignal() final;
    QSPI_HandleTypeDef& getQSPIHandle() final;

    static void registerIRQListener(QSPI_HandleTypeDef* hqspi, STM32QSPI& qspi);
    static ISTM32QuadSPIIRQListener& getIRQListener(QSPI_HandleTypeDef* hqspi);

    static constexpr size_t MIN_FLASH_SIZE = 32;

private:
    static constexpr uint32_t QSPI_TIMEOUT_MS = 10000;

    Signal cmdCplt;
    Signal rxCplt;
    Signal txCplt;
    Signal statusMatch;

    QSPI_HandleTypeDef m_hQSPI;
    QUADSPI_TypeDef* m_qspiInstance = nullptr;

    void initCommandStructure(QSPI_CommandTypeDef* sCommand);
    uint32_t dataNbLinesToHAL(uint8_t nbLines);
    uint32_t addrModeToHal(SFlashCommunicationInterfaceAddrMode addrMode);

    bool sendQSPICommand(QSPI_CommandTypeDef* cmd);
    bool sendQSPICommand_IT(QSPI_CommandTypeDef* cmd);
    bool transmitQSPIData(uint8_t* pData);
    bool transmitQSPIData_IT(uint8_t* pData);
    bool transmitQSPIDataDMA(uint8_t* pData);
    bool receiveQSPIData(uint8_t* pData);
    bool receiveQSPIDataDMA(uint8_t* pData);
    bool autoPollQSPI(QSPI_CommandTypeDef* cmd, QSPI_AutoPollingTypeDef* cfg);
    bool autoPollQSPI_IT(QSPI_CommandTypeDef* cmd, QSPI_AutoPollingTypeDef* cfg);
};

#endif /* SFLASH_STM32QSPI_HPP_ */
