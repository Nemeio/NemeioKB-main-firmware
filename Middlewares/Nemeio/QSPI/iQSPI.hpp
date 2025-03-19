/*
 * iQSPI.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_QSPI_IQSPI_HPP_
#define NEMEIO_QSPI_IQSPI_HPP_

#include <cstdint>
#include <cstddef>

#include "FlashStructures.hpp"

typedef enum IQSPIErrCode_t
{
    IQSPI_ERROR_SUCCESS,
    IQSPI_ERROR_TRANSFER_FAILED,
    IQSPI_ERROR_INVALID_PARAMETER,
    IQSPI_ERROR_UNKNOWN
} IQSPIErrCode;

class iQSPI
{
public:
    virtual IQSPIErrCode init(size_t size) = 0;
    virtual IQSPIErrCode deinit() = 0;

    virtual IQSPIErrCode transmit(FlashCommand& command) = 0;
    virtual IQSPIErrCode transmit(FlashCommand& command,
                                  uint8_t* txData,
                                  size_t size,
                                  bool bDMA = false) = 0;
    virtual IQSPIErrCode receive(FlashCommand& command,
                                 uint8_t* rxData,
                                 size_t size,
                                 bool bDMA = false) = 0;

    virtual IQSPIErrCode transmitAndWaitValue(FlashCommand& command, FlashWaitValue value) = 0;
};

#endif /* NEMEIO_QSPI_IQSPI_HPP_ */
