/*
 * MultipleChipSelector.h
 *
 *  Created on: May 12, 2017
 *      Author: Labo
 */

#ifndef SPI_CHIPSELECTOR_MULTIPLECHIPSELECTOR_MULTIPLECHIPSELECTOR_H_
#define SPI_CHIPSELECTOR_MULTIPLECHIPSELECTOR_MULTIPLECHIPSELECTOR_H_

#include "iChipSelector.h"
#include <vector>

class MultipleChipSelector : public iChipSelector
{
public:
    MultipleChipSelector();
    virtual ~MultipleChipSelector();

    std::vector<iChipSelector*> mVChipSelectors;

    void activate();
    void deactivate();
    void add(iChipSelector& chipSelector);


    void enable();
    void disable();
};

#endif /* SPI_CHIPSELECTOR_MULTIPLECHIPSELECTOR_MULTIPLECHIPSELECTOR_H_ */
