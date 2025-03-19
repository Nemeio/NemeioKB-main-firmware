/*
 * QSPISFlashCommunicationInterface.cpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#include <QSPISFlashCommunicationInterface.hpp>

QSPISFlashCommunicationInterface::QSPISFlashCommunicationInterface(iQSPI& qspi)
    : mQspi(qspi)
{
}

bool QSPISFlashCommunicationInterface::init(size_t size)
{
    return (mQspi.init(size) == IQSPI_ERROR_SUCCESS);
}

bool QSPISFlashCommunicationInterface::deinit()
{
    return (mQspi.deinit() == IQSPI_ERROR_SUCCESS);
}

bool QSPISFlashCommunicationInterface::transmit(FlashCommand& command)
{
    return (mQspi.transmit(command) == IQSPI_ERROR_SUCCESS);
}

bool QSPISFlashCommunicationInterface::transmit(FlashCommand& command,
                                                uint8_t* txData,
                                                size_t size,
                                                bool bDMA)
{
    return (mQspi.transmit(command, txData, size, bDMA) == IQSPI_ERROR_SUCCESS);
}

bool QSPISFlashCommunicationInterface::receive(FlashCommand& command,
                                               uint8_t* rxData,
                                               size_t size,
                                               bool bDMA)
{
    return (mQspi.receive(command, rxData, size, bDMA) == IQSPI_ERROR_SUCCESS);
}

bool QSPISFlashCommunicationInterface::transmitAndWaitValue(FlashCommand& command,
                                                            FlashWaitValue value)
{
    return (mQspi.transmitAndWaitValue(command, value) == IQSPI_ERROR_SUCCESS);
}
