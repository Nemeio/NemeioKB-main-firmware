/*
 * Signal.h
 *
 *  Created on: Mar 14, 2017
 *      Author: Labo
 */

#ifndef SIGNAL_SIGNAL_H_
#define SIGNAL_SIGNAL_H_

#include "DataSignal.hpp"
#include "ISignal.hpp"
#include <cstdint>

class Signal : public ISignal
{
public:
    Signal() = default;
    ~Signal() override = default;

    bool wait(DelayMs delay) override;
    void clear() override;
    void setFromISR() override;
    void set() override;
    void set(DelayMs delay) override;

private:
    DataSignal<uint8_t, 1> mDataSignal;
};

#endif /* SIGNAL_SIGNAL_H_ */
