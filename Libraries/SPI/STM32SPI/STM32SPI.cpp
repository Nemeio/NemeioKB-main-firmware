/*
 * STM32SPI.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: Labo
 */

#include "STM32SPI.h"
#include "cmsis_os.h"
#include <string.h>
#include <algorithm>
#include "NullChipSelector.h"

#define DMA_LEVEL 20

extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi3_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;


typedef enum SPIInstanceIdx_t
{
    SPI_INSTANCE_IDX_SPI1 = 0,
    SPI_INSTANCE_IDX_SPI2,
    SPI_INSTANCE_IDX_SPI3
} SPIInstanceIdx;

/**
* @brief This function handles SPI1 global interrupt.
*/
extern "C" void SPI1_IRQHandler(void)
{
    /* USER CODE BEGIN SPI1_IRQn 0 */

    /* USER CODE END SPI1_IRQn 0 */
    STM32SPI::SPIIRQHandler(SPI1);
    /* USER CODE BEGIN SPI1_IRQn 1 */

    /* USER CODE END SPI1_IRQn 1 */
}

extern "C" void SPI2_IRQHandler(void)
{
    /* USER CODE BEGIN SPI1_IRQn 0 */

    /* USER CODE END SPI1_IRQn 0 */
    STM32SPI::SPIIRQHandler(SPI2);
    /* USER CODE BEGIN SPI1_IRQn 1 */

    /* USER CODE END SPI1_IRQn 1 */
}

extern "C" void SPI3_IRQHandler(void)
{
    /* USER CODE BEGIN SPI1_IRQn 0 */

    /* USER CODE END SPI1_IRQn 0 */
    STM32SPI::SPIIRQHandler(SPI3);
    /* USER CODE BEGIN SPI1_IRQn 1 */

    /* USER CODE END SPI1_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel4 global interrupt.
*/
extern "C" void DMA1_Channel4_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

    /* USER CODE END DMA1_Channel4_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_spi2_rx);
    /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

    /* USER CODE END DMA1_Channel4_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel5 global interrupt.
*/
extern "C" void DMA1_Channel5_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

    /* USER CODE END DMA1_Channel5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_spi2_tx);
    /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

    /* USER CODE END DMA1_Channel5_IRQn 1 */
}


/**
* @brief This function handles DMA2 channel1 global interrupt.
*/
extern "C" void DMA2_Channel1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Channel1_IRQn 0 */

    /* USER CODE END DMA2_Channel1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_spi3_rx);
    /* USER CODE BEGIN DMA2_Channel1_IRQn 1 */

    /* USER CODE END DMA2_Channel1_IRQn 1 */
}

/**
* @brief This function handles DMA2 channel2 global interrupt.
*/
extern "C" void DMA2_Channel2_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Channel2_IRQn 0 */

    /* USER CODE END DMA2_Channel2_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_spi3_tx);
    /* USER CODE BEGIN DMA2_Channel2_IRQn 1 */

    /* USER CODE END DMA2_Channel2_IRQn 1 */
}


extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    STM32SPI::SPITxCpltCallback(hspi);
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
{
    STM32SPI::SPIRxCpltCallback(hspi);
}

extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi)
{
    STM32SPI::SPITxRxCpltCallback(hspi);
}

extern "C" void HAL_SPI_ErrorCallback(SPI_HandleTypeDef* hspi)
{
    STM32SPI::SPIErrorCallback(hspi);
}

SPISignals* STM32SPI::sPSPISignals[gNbSPIInstances] = {0};

SPI_HandleTypeDef* STM32SPI::sPSPIHandles[gNbSPIInstances] = {0};

bool STM32SPI::sIsInstanceInitialized[gNbSPIInstances] = {0};

STM32SPI::~STM32SPI()
{
    uint32_t instanceIndex = getIndexFromInstance(mInstance);

    sPSPISignals[instanceIndex] = NULL;

    sPSPIHandles[instanceIndex] = NULL;
}

STM32SPI::STM32SPI(SPI_TypeDef* instance)
    : mInstance(instance)
{
    uint32_t instanceIndex = getIndexFromInstance(mInstance);

    if(NULL == sPSPISignals[instanceIndex])
    {
        sPSPISignals[instanceIndex] = &mSPISignals;
    }

    memset(&mHSPI, 0, sizeof(mHSPI));

    if(NULL == sPSPIHandles[instanceIndex])
    {
        sPSPIHandles[instanceIndex] = &mHSPI;
    }
}

ISPIErrCode STM32SPI::transmitUint16Max(const uint8_t txData[], size_t size)
{
    if(size > UINT16_MAX)
    {
        return ISPI_ERROR_INVALID_PARAMETER;
    }

    mSPISignals.spiEvt.clear();

    uint8_t* txDataForHAL = const_cast<uint8_t*>(txData);
    uint16_t sizeU16 = static_cast<uint16_t>(size);

    // No DMA
    if(size < DMA_LEVEL)
    {
        if(HAL_OK != HAL_SPI_Transmit_IT(&mHSPI, txDataForHAL, sizeU16))
        {
            return ISPI_ERROR_TRANSFER_FAILED;
        }
    }
    else // DMA
    {
        if(HAL_OK != HAL_SPI_Transmit_DMA(&mHSPI, txDataForHAL, sizeU16))
        {
            return ISPI_ERROR_TRANSFER_FAILED;
        }
    }

    ISPIErrCode ret = ISPI_ERROR_TRANSFER_FAILED;
    SPIEvent evt = SPIEVT_UNDEFINED;

    while(1)
    {
        if(mSPISignals.spiEvt.waitData(gSPICommTimeoutMs, evt))
        {
            if(SPIEVT_TX_CPLT == evt)
            {
                ret = ISPI_ERROR_SUCCESS;
                break;
            }
            else if(SPIEVT_ERROR == evt)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return ret;
}

ISPIErrCode STM32SPI::specificTransmit(const uint8_t txData[], size_t size)
{
    while(size > 0)
    {
        uint16_t transferSizeWords = std::min(size, (size_t) UINT16_MAX);

        ISPIErrCode spiErr = transmitUint16Max(txData, transferSizeWords);

        if(ISPI_ERROR_SUCCESS != spiErr)
        {
            return spiErr;
        }

        size -= transferSizeWords;
        txData += transferSizeWords;
    }

    return ISPI_ERROR_SUCCESS;
}

ISPIErrCode STM32SPI::specificReceive(uint8_t rxData[], size_t size, bool bDMA)
{
    if(size > UINT16_MAX)
    {
        return ISPI_ERROR_INVALID_PARAMETER;
    }

    mSPISignals.spiEvt.clear();

    uint16_t sizeU16 = static_cast<uint16_t>(size);

    // No DMA
    if(!bDMA || size < DMA_LEVEL)
    {
        if(HAL_OK != HAL_SPI_Receive_IT(&mHSPI, rxData, sizeU16))
        {
            return ISPI_ERROR_TRANSFER_FAILED;
        }
    }
    else // DMA
    {
        if(HAL_OK != HAL_SPI_Receive_DMA(&mHSPI, rxData, sizeU16))
        {
            return ISPI_ERROR_TRANSFER_FAILED;
        }
    }

    ISPIErrCode ret = ISPI_ERROR_TRANSFER_FAILED;
    SPIEvent evt = SPIEVT_UNDEFINED;

    while(1)
    {
        if(mSPISignals.spiEvt.waitData(gSPICommTimeoutMs, evt))
        {
            if(SPIEVT_RX_CPLT == evt)
            {
                ret = ISPI_ERROR_SUCCESS;
                break;
            }
            else if(SPIEVT_ERROR == evt)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return ret;
}

ISPIErrCode STM32SPI::specificTransmitReceive(const uint8_t txData[], uint8_t rxData[], size_t size)
{
    if(size > UINT16_MAX)
    {
        return ISPI_ERROR_INVALID_PARAMETER;
    }

    mSPISignals.spiEvt.clear();

    uint8_t* txDataForHAL = const_cast<uint8_t*>(txData);
    uint16_t sizeU16 = static_cast<uint16_t>(size);

    // No DMA
    if(size < DMA_LEVEL)
    {
        if(HAL_OK != HAL_SPI_TransmitReceive_IT(&mHSPI, txDataForHAL, rxData, sizeU16))
        {
            return ISPI_ERROR_TRANSFER_FAILED;
        }
    }
    else // DMA
    {
        if(HAL_OK != HAL_SPI_TransmitReceive_DMA(&mHSPI, txDataForHAL, rxData, sizeU16))
        {
            return ISPI_ERROR_TRANSFER_FAILED;
        }
    }

    ISPIErrCode ret = ISPI_ERROR_TRANSFER_FAILED;
    SPIEvent evt = SPIEVT_UNDEFINED;

    while(1)
    {
        if(mSPISignals.spiEvt.waitData(gSPICommTimeoutMs, evt))
        {
            if(SPIEVT_TXRX_CPLT == evt)
            {
                ret = ISPI_ERROR_SUCCESS;
                break;
            }
            else if(SPIEVT_ERROR == evt)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return ret;
}

ISPIErrCode STM32SPI::init(SPIConfig spiConfig)
{
    uint32_t instanceIndex = getIndexFromInstance(mInstance);

    ASSERT(!sIsInstanceInitialized[instanceIndex]);

    mHSPI.Instance = mInstance;
    mHSPI.Init.Mode = SPI_MODE_MASTER;
    mHSPI.Init.Direction = SPI_DIRECTION_2LINES;
    mHSPI.Init.DataSize = convertDataSize(spiConfig.dataSize);
    mHSPI.Init.CLKPolarity = convertPolarity(spiConfig.clkPolarity);
    mHSPI.Init.CLKPhase = convertPhase(spiConfig.clkPhase);
    mHSPI.Init.NSS = SPI_NSS_SOFT;
    mHSPI.Init.BaudRatePrescaler = computePrescaler(spiConfig.clock);
    mHSPI.Init.FirstBit = convertFirstBit(spiConfig.firstBit);
    mHSPI.Init.TIMode = SPI_TIMODE_DISABLE;
    mHSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    mHSPI.Init.CRCPolynomial = 7;
    mHSPI.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    mHSPI.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;

    if(HAL_SPI_Init(&mHSPI) != HAL_OK)
    {
        return ISPI_ERROR_UNKNOWN;
    }

    sIsInstanceInitialized[instanceIndex] = true;

    return ISPI_ERROR_SUCCESS;
}

uint32_t STM32SPI::convertDataSize(SPIDataSize dataSize)
{
    switch(dataSize)
    {
    case ISPI_DATASIZE_4BIT:
        return SPI_DATASIZE_4BIT;
    case ISPI_DATASIZE_5BIT:
        return SPI_DATASIZE_5BIT;
    case ISPI_DATASIZE_6BIT:
        return SPI_DATASIZE_6BIT;
    case ISPI_DATASIZE_7BIT:
        return SPI_DATASIZE_7BIT;
    case ISPI_DATASIZE_8BIT:
        return SPI_DATASIZE_8BIT;
    case ISPI_DATASIZE_9BIT:
        return SPI_DATASIZE_9BIT;
    case ISPI_DATASIZE_10BIT:
        return SPI_DATASIZE_10BIT;
    case ISPI_DATASIZE_11BIT:
        return SPI_DATASIZE_11BIT;
    case ISPI_DATASIZE_12BIT:
        return SPI_DATASIZE_12BIT;
    case ISPI_DATASIZE_13BIT:
        return SPI_DATASIZE_13BIT;
    case ISPI_DATASIZE_14BIT:
        return SPI_DATASIZE_14BIT;
    case ISPI_DATASIZE_15BIT:
        return SPI_DATASIZE_15BIT;
    case ISPI_DATASIZE_16BIT:
        return SPI_DATASIZE_16BIT;
    default:
        return SPI_DATASIZE_8BIT;
    }
}

uint32_t STM32SPI::convertPolarity(SPIPolarity polarity)
{
    switch(polarity)
    {
    case ISPI_POLARITY_LOW:
        return SPI_POLARITY_LOW;
    case ISPI_POLARITY_HIGH:
        return SPI_POLARITY_HIGH;
    default:
        return SPI_POLARITY_LOW;
    }
}

uint32_t STM32SPI::convertFirstBit(SPIFirstBit firstBit)
{
    switch(firstBit)
    {
    case ISPI_FIRSTBIT_MSB:
        return SPI_FIRSTBIT_MSB;
    case ISPI_FIRSTBIT_LSB:
        return SPI_FIRSTBIT_LSB;
    default:
        return SPI_FIRSTBIT_MSB;
    }
}

uint32_t STM32SPI::convertPhase(SPIPhase phase)
{
    switch(phase)
    {
    case ISPI_PHASE_1EDGE:
        return SPI_PHASE_1EDGE;
    case ISPI_PHASE_2EDGE:
        return SPI_PHASE_2EDGE;
    default:
        return SPI_PHASE_1EDGE;
    }
}

SPISignals* STM32SPI::getSPISignalsFromInstance(SPI_TypeDef* instance)
{
    SPISignals* pSPISignals = NULL;
    uint32_t instanceIndex = getIndexFromInstance(instance);

    if(instanceIndex < gNbSPIInstances)
    {
        pSPISignals = sPSPISignals[instanceIndex];
    }

    return pSPISignals;
}

void STM32SPI::SPITxCpltCallback(SPI_HandleTypeDef* hspi)
{
    SPISignals* pSPISignals = getSPISignalsFromInstance(hspi->Instance);
    if(pSPISignals != NULL)
    {
        enum SPIEvent event = SPIEVT_TX_CPLT;
        pSPISignals->spiEvt.setFromISRData(event);
    }
}

void STM32SPI::SPIRxCpltCallback(SPI_HandleTypeDef* hspi)
{
    SPISignals* pSPISignals = getSPISignalsFromInstance(hspi->Instance);
    if(pSPISignals != NULL)
    {
        enum SPIEvent event = SPIEVT_RX_CPLT;
        pSPISignals->spiEvt.setFromISRData(event);
    }
}

void STM32SPI::SPITxRxCpltCallback(SPI_HandleTypeDef* hspi)
{
    SPISignals* pSPISignals = getSPISignalsFromInstance(hspi->Instance);
    if(pSPISignals != NULL)
    {
        enum SPIEvent event = SPIEVT_TXRX_CPLT;
        pSPISignals->spiEvt.setFromISRData(event);
    }
}

void STM32SPI::SPIErrorCallback(SPI_HandleTypeDef* hspi)
{
    SPISignals* pSPISignals = getSPISignalsFromInstance(hspi->Instance);
    if(pSPISignals != NULL)
    {
        enum SPIEvent event = SPIEVT_ERROR;
        pSPISignals->spiEvt.setFromISRData(event);
    }
}

void STM32SPI::SPIIRQHandler(SPI_TypeDef* instance)
{
    SPI_HandleTypeDef* hSPI = getSPIHandleFromInstance(instance);
    if(hSPI != NULL)
    {
        HAL_SPI_IRQHandler(hSPI);
    }
}

uint32_t STM32SPI::getIndexFromInstance(SPI_TypeDef* instance)
{
    uint32_t index = SPI_INSTANCE_IDX_SPI1;

    if(SPI1 == instance)
    {
        index = SPI_INSTANCE_IDX_SPI1;
    }
    else if(SPI2 == instance)
    {
        index = SPI_INSTANCE_IDX_SPI2;
    }
    else if(SPI3 == instance)
    {
        index = SPI_INSTANCE_IDX_SPI3;
    }
    else
    {
        ASSERT(false);
    }

    return index;
}

uint32_t STM32SPI::computePrescaler(uint32_t clock)
{
    uint32_t apbFreq = 0;
    uint32_t prescaler = 0;

    if(SPI1 == mInstance)
    {
        apbFreq = HAL_RCC_GetPCLK2Freq();
    }
    else
    {
        apbFreq = HAL_RCC_GetPCLK1Freq();
    }

    prescaler = apbFreq / clock;

    switch(prescaler)
    {
    case 2:
        return SPI_BAUDRATEPRESCALER_2;
    case 4:
        return SPI_BAUDRATEPRESCALER_4;
    case 8:
        return SPI_BAUDRATEPRESCALER_8;
    case 16:
        return SPI_BAUDRATEPRESCALER_16;
    case 32:
        return SPI_BAUDRATEPRESCALER_32;
    case 64:
        return SPI_BAUDRATEPRESCALER_64;
    case 128:
        return SPI_BAUDRATEPRESCALER_128;
    case 256:
        return SPI_BAUDRATEPRESCALER_256;
    default:
        ASSERT(false);
    }
}

SPI_HandleTypeDef* STM32SPI::getSPIHandleFromInstance(SPI_TypeDef* instance)
{
    SPI_HandleTypeDef* hSPI = NULL;
    uint32_t instanceIndex = getIndexFromInstance(instance);

    if(instanceIndex < gNbSPIInstances)
    {
        hSPI = sPSPIHandles[instanceIndex];
    }

    return hSPI;
}

ISPIErrCode STM32SPI::deinit()
{
    uint32_t instanceIndex = getIndexFromInstance(mInstance);

    if((instanceIndex < gNbSPIInstances) && sIsInstanceInitialized[instanceIndex])
    {
        if(HAL_SPI_DeInit(&mHSPI) != HAL_OK)
        {
            return ISPI_ERROR_UNKNOWN;
        }

        sIsInstanceInitialized[instanceIndex] = false;
    }

    return ISPI_ERROR_SUCCESS;
}


ISPIErrCode STM32SPI::clearMosi()
{
    // After a transfer, MOSI state = MSB of last 4 bytes if sent >= 4 bytes
    // The hack to force MOSI to 0 is sending a zero 4-bytes buffer without selecting chip
    NullChipSelector nullChipSelector;
    uint32_t zero = 0;
    return transmit(reinterpret_cast<const uint8_t*>(&zero), sizeof(zero), nullChipSelector);
}
