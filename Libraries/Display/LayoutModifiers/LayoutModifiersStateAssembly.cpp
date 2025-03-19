/*
 * LayoutModifiersStateAssembly.cpp
 *
 *  Created on: 3 juin 2020
 *      Author: eskoric
 */

#include "LayoutModifiersStateAssembly.hpp"

LayoutModifiersStateAssembly::LayoutModifiersStateAssembly(ILayoutModifierListener& listener)
    : mLayoutModifiersState(mMutex, listener)
{
}

LayoutModifiersState& LayoutModifiersStateAssembly::getLayoutModifiersStateItem()
{
    return mLayoutModifiersState;
}
