/*
 * STM32SPI.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Labo
 */

#ifndef SPI_STM32SPI_H_
#define SPI_STM32SPI_H_

#include "AbstractSPI.h"
#include "stm32l4xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "DataSignal.hpp"
#include "cmsis_os.h"

enum SPIEvent
{
    SPIEVT_UNDEFINED = 0,
    SPIEVT_TX_CPLT,
    SPIEVT_RX_CPLT,
    SPIEVT_TXRX_CPLT,
    SPIEVT_ERROR
};


typedef struct SPISignals_t
{
    DataSignal<enum SPIEvent, 1> spiEvt;
} SPISignals;

static const uint8_t gNbSPIInstances = 3;

class STM32SPI : public AbstractSPI
{
public:
    virtual ~STM32SPI();
    STM32SPI(SPI_TypeDef* instance);

    STM32SPI(STM32SPI&) = delete;
    STM32SPI& operator=(const STM32SPI&) = delete;
    STM32SPI(STM32SPI&&) = delete;
    STM32SPI const& operator=(STM32SPI&&) = delete;

    static void SPIIRQHandler(SPI_TypeDef* instance);
    static void SPITxCpltCallback(SPI_HandleTypeDef* hspi);
    static void SPIRxCpltCallback(SPI_HandleTypeDef* hspi);
    static void SPITxRxCpltCallback(SPI_HandleTypeDef* hspi);
    static void SPIErrorCallback(SPI_HandleTypeDef* hspi);

    /*	ISPIErrCode transmit(uint8_t txData[], size_t size);
	ISPIErrCode receive(uint8_t rxData[], size_t size);
	ISPIErrCode transmitReceive(uint8_t txData[], uint8_t rxData[],
			size_t size);*/
    ISPIErrCode init(SPIConfig spiConfig) override;
    ISPIErrCode deinit() override;

    ISPIErrCode clearMosi() override;

private:
    static SPISignals* sPSPISignals[gNbSPIInstances];
    static SPI_HandleTypeDef* sPSPIHandles[gNbSPIInstances];
    static bool sIsInstanceInitialized[gNbSPIInstances];

    static const uint32_t gSPICommTimeoutMs = 5000;

    SPI_TypeDef* mInstance;
    SPI_HandleTypeDef mHSPI;
    SPISignals mSPISignals;

    ISPIErrCode specificTransmit(const uint8_t txData[], size_t size) override;
    ISPIErrCode transmitUint16Max(const uint8_t txData[], size_t size);
    ISPIErrCode specificReceive(uint8_t rxData[], size_t size, bool bDMA = false) override;
    ISPIErrCode specificTransmitReceive(const uint8_t txData[],
                                        uint8_t rxData[],
                                        size_t size) override;

    uint32_t convertDataSize(SPIDataSize dataSize);
    uint32_t convertFirstBit(SPIFirstBit firstBit);
    uint32_t convertPolarity(SPIPolarity polarity);
    uint32_t convertPhase(SPIPhase phase);
    uint32_t computePrescaler(uint32_t clock);

    static SPISignals* getSPISignalsFromInstance(SPI_TypeDef* instance);
    static uint32_t getIndexFromInstance(SPI_TypeDef* instance);

    static SPI_HandleTypeDef* getSPIHandleFromInstance(SPI_TypeDef* instance);
};

#endif /* SPI_STM32SPI_H_ */
