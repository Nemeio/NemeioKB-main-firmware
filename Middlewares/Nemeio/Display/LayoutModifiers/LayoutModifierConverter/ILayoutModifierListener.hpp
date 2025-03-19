/*
 * ILayoutModifierListener.hpp
 *
 *  Created on: 1 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_ILAYOUTMODIFIERLISTENER_HPP_
#define NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_ILAYOUTMODIFIERLISTENER_HPP_

#include "LayoutModifiersDefs.hpp"

class ILayoutModifierListener
{
public:
    ILayoutModifierListener() = default;
    virtual ~ILayoutModifierListener() = default;

    virtual void layoutModifiersStateChanged(
        const LayoutModifiersDefs::LayoutModifierFlags& flags) = 0;
};

#endif /* NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_ILAYOUTMODIFIERLISTENER_HPP_ */
