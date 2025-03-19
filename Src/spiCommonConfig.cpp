/*
 * spiCommonConfig.cpp
 *
 *  Created on: May 11, 2017
 *      Author: Labo
 */
#include "iSPI.h"

// SPI Master configuration for Holitech Screen and BLE SoC
extern const SPIConfig gSPICommonConfig = {
    2500000,            // 2 MHz SPI clock}
    ISPI_DATASIZE_8BIT, // 8-bit data size
    ISPI_FIRSTBIT_MSB,  // MSB first
    ISPI_POLARITY_LOW,  // CPOL = 0
    ISPI_PHASE_1EDGE    // CPHA = 0
};
