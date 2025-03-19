/*
 * AbstractSPI.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Labo
 */

#ifndef SPI_ABSTRACTSPI_ABSTRACTSPI_H_
#define SPI_ABSTRACTSPI_ABSTRACTSPI_H_

#include "common.h"
#include "iSPI.h"
#include "cmsis_os.h"
#include "Mutex.hpp"

class AbstractSPI : public iSPI
{
public:
    AbstractSPI() = default;
    virtual ~AbstractSPI() = default;

    ISPIErrCode transmit(const uint8_t txData[],
                         size_t size,
                         iChipSelector& chipSelector,
                         bool bEndTransfer = true) override;
    ISPIErrCode receive(uint8_t rxData[],
                        size_t size,
                        iChipSelector& chipSelector,
                        bool bEndTransfer = true,
                        bool bDMA = false) override;
    ISPIErrCode transmitReceive(const uint8_t txData[],
                                uint8_t rxData[],
                                size_t size,
                                iChipSelector& chipSelector,
                                bool bEndTransfer = true) override;

    ISPIErrCode endTransfer(iChipSelector& chipSelector) override;

protected:
    static const uint32_t gSPITimeoutMs = osWaitForever;

    virtual ISPIErrCode specificTransmit(const uint8_t txData[], size_t size) = 0;
    virtual ISPIErrCode specificReceive(uint8_t rxData[], size_t size, bool bDMA = false) = 0;
    virtual ISPIErrCode specificTransmitReceive(const uint8_t txData[],
                                                uint8_t rxData[],
                                                size_t size) = 0;

private:
    Mutex mSPIMutex;
};

#endif /* SPI_ABSTRACTSPI_ABSTRACTSPI_H_ */
