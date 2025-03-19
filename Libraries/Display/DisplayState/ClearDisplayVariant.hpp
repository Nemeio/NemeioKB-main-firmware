/*
 * ClearDisplayVariant.hpp
 *
 *  Created on: Sep 26, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_CLEARDISPLAYVARIANT_HPP_
#define DISPLAY_DISPLAYSTATE_CLEARDISPLAYVARIANT_HPP_

#include "MainDisplayVariant.hpp"
#include "IScreen.hpp"

class ClearDisplayVariant : public MainDisplayVariant
{
public:
    ClearDisplayVariant();
    virtual ~ClearDisplayVariant() = default;

    void draw(IScreen& screen, uint8_t fbNum) final;

    bool operator==(const ClearDisplayVariant& variant) const;
};

#endif /* DISPLAY_DISPLAYSTATE_CLEARDISPLAYVARIANT_HPP_ */
