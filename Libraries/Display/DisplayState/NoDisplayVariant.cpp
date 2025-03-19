/*
 * NoDisplayState.cpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#include "NoDisplayVariant.hpp"

NoDisplayVariant::NoDisplayVariant()
    : MainDisplayVariant(MainDisplayVariantId::NO_DISPLAY)
{
}

void NoDisplayVariant::draw(IScreen& screen, uint8_t fbNum)
{
    // Nothing to display
}

bool NoDisplayVariant::operator==(const NoDisplayVariant& variant) const
{
    return true;
}
