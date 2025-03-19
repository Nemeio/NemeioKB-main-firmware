/*
 * ClearDisplayVariant.cpp
 *
 *  Created on: Sep 26, 2019
 *      Author: Labo
 */

#include "ClearDisplayVariant.hpp"

ClearDisplayVariant::ClearDisplayVariant()
    : MainDisplayVariant(MainDisplayVariantId::CLEAR)
{
}

void ClearDisplayVariant::draw(IScreen& screen, uint8_t fbNum)
{
    screen.getAssociatedScreenController().clearScreen(screen);
}

bool ClearDisplayVariant::operator==(const ClearDisplayVariant& variant) const
{
    return true;
}
