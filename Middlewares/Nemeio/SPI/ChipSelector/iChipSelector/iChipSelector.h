/*
 * iChipSelector.h
 *
 *  Created on: May 12, 2017
 *      Author: Labo
 */

#ifndef SPI_CHIPSELECTOR_ICHIPSELECTOR_ICHIPSELECTOR_H_
#define SPI_CHIPSELECTOR_ICHIPSELECTOR_ICHIPSELECTOR_H_

class iChipSelector
{
public:
    iChipSelector();
    virtual ~iChipSelector();

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};

#endif /* SPI_CHIPSELECTOR_ICHIPSELECTOR_ICHIPSELECTOR_H_ */
