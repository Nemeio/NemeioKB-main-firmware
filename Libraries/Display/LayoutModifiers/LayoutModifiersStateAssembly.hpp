/*
 * LayoutModifiersStateAssembly.hpp
 *
 *  Created on: 3 juin 2020
 *      Author: eskoric
 */

#ifndef DISPLAY_LAYOUTMODIFIERS_LAYOUTMODIFIERSSTATEASSEMBLY_HPP_
#define DISPLAY_LAYOUTMODIFIERS_LAYOUTMODIFIERSSTATEASSEMBLY_HPP_

#include "LayoutModifiersState.hpp"
#include "RecursiveMutex.hpp"

class LayoutModifiersStateAssembly
{
public:
    LayoutModifiersStateAssembly(ILayoutModifierListener& listener);
    virtual ~LayoutModifiersStateAssembly() = default;

    LayoutModifiersState& getLayoutModifiersStateItem();

private:
    RecursiveMutex mMutex;
    LayoutModifiersState mLayoutModifiersState;
};

#endif /* DISPLAY_LAYOUTMODIFIERS_LAYOUTMODIFIERSSTATEASSEMBLY_HPP_ */
