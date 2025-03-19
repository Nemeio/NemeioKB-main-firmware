/*
 * NullChipSelector.h
 *
 *  Created on: 1 juin 2017
 *      Author: cgarnier
 */

#ifndef SPI_CHIPSELECTOR_NULLCHIPSELECTOR_NULLCHIPSELECTOR_H_
#define SPI_CHIPSELECTOR_NULLCHIPSELECTOR_NULLCHIPSELECTOR_H_

#include "GPIOPins.h"
#include "IGPIOController.hpp"
#include "iChipSelector.h"
class NullChipSelector : public iChipSelector
{
public:
    NullChipSelector() = default;
    virtual ~NullChipSelector() = default;
    void activate();
    void deactivate();
};

#endif /* SPI_CHIPSELECTOR_NULLCHIPSELECTOR_NULLCHIPSELECTOR_H_ */
