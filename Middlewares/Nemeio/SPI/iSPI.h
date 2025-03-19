/*
 * iSPI.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Labo
 */

#ifndef SPI_ISPI_ISPI_H_
#define SPI_ISPI_ISPI_H_

#include "common.h"
#include "IGPIOController.hpp"
#include "iChipSelector.h"

typedef enum ISPIErrCode_t
{
    ISPI_ERROR_SUCCESS,
    ISPI_ERROR_TRANSFER_FAILED,
    ISPI_ERROR_INVALID_PARAMETER,
    ISPI_ERROR_UNKNOWN
} ISPIErrCode;

typedef enum ISPIMode_t
{
    ISPI_MODE_0 = 0,
    ISPI_MODE_1,
    ISPI_MODE_2,
    ISPI_MODE_3
} SPIMode;

typedef enum SPIDataSize_t
{
    ISPI_DATASIZE_4BIT,
    ISPI_DATASIZE_5BIT,
    ISPI_DATASIZE_6BIT,
    ISPI_DATASIZE_7BIT,
    ISPI_DATASIZE_8BIT,
    ISPI_DATASIZE_9BIT,
    ISPI_DATASIZE_10BIT,
    ISPI_DATASIZE_11BIT,
    ISPI_DATASIZE_12BIT,
    ISPI_DATASIZE_13BIT,
    ISPI_DATASIZE_14BIT,
    ISPI_DATASIZE_15BIT,
    ISPI_DATASIZE_16BIT
} SPIDataSize;

typedef enum SPIFirstBit_t
{
    ISPI_FIRSTBIT_MSB,
    ISPI_FIRSTBIT_LSB
} SPIFirstBit;

typedef enum SPIPolarity_t
{
    ISPI_POLARITY_LOW,
    ISPI_POLARITY_HIGH
} SPIPolarity;

typedef enum SPIPhase_t
{
    ISPI_PHASE_1EDGE,
    ISPI_PHASE_2EDGE
} SPIPhase;

typedef struct SPIConfig_t
{
    uint32_t clock;
    SPIDataSize dataSize;
    SPIFirstBit firstBit;
    SPIPolarity clkPolarity;
    SPIPhase clkPhase;
} SPIConfig;

class iSPI
{
public:
    iSPI() = default;
    virtual ~iSPI() = default;

    virtual ISPIErrCode transmit(const uint8_t txData[],
                                 size_t size,
                                 iChipSelector& chipSelector,
                                 bool bEndTransfer = true) = 0;
    virtual ISPIErrCode receive(uint8_t rxData[],
                                size_t size,
                                iChipSelector& chipSelector,
                                bool bEndTransfer = true,
                                bool bDMA = false) = 0;
    virtual ISPIErrCode transmitReceive(const uint8_t txData[],
                                        uint8_t rxData[],
                                        size_t size,
                                        iChipSelector& chipSelector,
                                        bool bEndTransfer = true) = 0;
    virtual ISPIErrCode init(SPIConfig spiConfig) = 0;
    virtual ISPIErrCode deinit() = 0;
    virtual ISPIErrCode endTransfer(iChipSelector& chipSelector) = 0;
    virtual ISPIErrCode clearMosi() = 0;
};

#endif /* SPI_ISPI_ISPI_H_ */
