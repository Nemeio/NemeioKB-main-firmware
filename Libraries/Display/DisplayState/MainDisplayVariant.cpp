/*
 * MainDisplayVariant.cpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#include "MainDisplayVariant.hpp"

MainDisplayVariant::MainDisplayVariant(MainDisplayVariantId id)
    : mId(id)
{
}

bool MainDisplayVariant::isSameVariant(const MainDisplayVariant& variant) const
{
    return variant.mId == mId;
}

MainDisplayVariant::MainDisplayVariantId MainDisplayVariant::getId() const
{
    return mId;
}

const ILayoutModifierStrategy& MainDisplayVariant::getModifierStrategy() const
{
    return mNoModifierStrategy;
}
