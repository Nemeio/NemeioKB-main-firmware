/*
 * SingleChipSelector.h
 *
 *  Created on: May 12, 2017
 *      Author: Labo
 */

#ifndef SPI_CHIPSELECTOR_SINGLECHIPSELECTOR_SINGLECHIPSELECTOR_H_
#define SPI_CHIPSELECTOR_SINGLECHIPSELECTOR_SINGLECHIPSELECTOR_H_

#include "GPIOPins.h"
#include "IGPIOController.hpp"
#include "iChipSelector.h"

class SingleChipSelector : public iChipSelector
{
public:
    SingleChipSelector(IGPIOController& gpioController, GPIOPin csNPin);
    virtual ~SingleChipSelector();

    void activate() override;
    void deactivate() override;

private:
    IGPIOController& mGPIOController;
    GPIOPin mCSNPin;
};

#endif /* SPI_CHIPSELECTOR_SINGLECHIPSELECTOR_SINGLECHIPSELECTOR_H_ */
