/*
 * ISTM32QuadSPIIRQListener.hpp
 *
 *  Created on: 2 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_ISTM32QUADSPIIRQLISTENER_HPP_
#define SFLASH_ISTM32QUADSPIIRQLISTENER_HPP_

#include "Signal.hpp"

class ISTM32QuadSPIIRQListener
{
public:
    virtual Signal& getCmdCpltSignal() = 0;
    virtual Signal& getTxCpltSignal() = 0;
    virtual Signal& getRxCpltSignal() = 0;
    virtual Signal& getStatusMatchSignal() = 0;
    virtual QSPI_HandleTypeDef& getQSPIHandle() = 0;
};


#endif /* SFLASH_ISTM32QUADSPIIRQLISTENER_HPP_ */
