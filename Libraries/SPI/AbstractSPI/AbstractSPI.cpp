/*
 * AbstractSPI.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: Labo
 */

#include "AbstractSPI.h"
#include "AutoLock.h"

ISPIErrCode AbstractSPI::transmit(const uint8_t txData[],
                                  size_t size,
                                  iChipSelector& chipSelector,
                                  bool bEndTransfer)
{
    AutoLock autoLock(mSPIMutex, gSPITimeoutMs);
    ISPIErrCode errCode = ISPI_ERROR_SUCCESS;

    chipSelector.activate();
    errCode = specificTransmit(txData, size);
    if(bEndTransfer)
    {
        chipSelector.deactivate();
    }

    return errCode;
}

ISPIErrCode AbstractSPI::endTransfer(iChipSelector& chipSelector)
{
    AutoLock autoLock(mSPIMutex, gSPITimeoutMs);

    ISPIErrCode errCode = ISPI_ERROR_SUCCESS;

    chipSelector.deactivate();

    return errCode;
}

ISPIErrCode AbstractSPI::receive(
    uint8_t rxData[], size_t size, iChipSelector& chipSelector, bool bEndTransfer, bool bDMA)
{
    AutoLock autoLock(mSPIMutex, gSPITimeoutMs);

    ISPIErrCode errCode = ISPI_ERROR_SUCCESS;

    chipSelector.activate();
    errCode = specificReceive(rxData, size, bDMA);
    if(bEndTransfer)
    {
        chipSelector.deactivate();
    }

    return errCode;
}

ISPIErrCode AbstractSPI::transmitReceive(const uint8_t txData[],
                                         uint8_t rxData[],
                                         size_t size,
                                         iChipSelector& chipSelector,
                                         bool bEndTransfer)
{
    AutoLock autoLock(mSPIMutex, gSPITimeoutMs);

    ISPIErrCode errCode = ISPI_ERROR_SUCCESS;

    chipSelector.activate();
    errCode = specificTransmitReceive(txData, rxData, size);
    if(bEndTransfer)
    {
        chipSelector.deactivate();
    }

    return errCode;
}
