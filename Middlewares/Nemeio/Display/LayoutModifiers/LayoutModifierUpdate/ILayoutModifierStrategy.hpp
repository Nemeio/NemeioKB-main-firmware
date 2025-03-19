/*
 * ILayoutModifierStrategy.hpp
 *
 *  Created on: 2 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_ILAYOUTMODIFIERSTRATEGY_HPP_
#define NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_ILAYOUTMODIFIERSTRATEGY_HPP_

#include "LayoutModifiersDefs.hpp"
#include <functional>

class ILayoutModifierStrategy
{
public:
    ILayoutModifierStrategy() = default;
    virtual ~ILayoutModifierStrategy() = default;

    virtual LayoutModifiersDefs::FrameBufferIndex convertFlagsToFbNum(
        const LayoutModifiersDefs::LayoutModifierFlags flags) const = 0;
    virtual void updateModifiers(const LayoutModifiersDefs::LayoutModifierFlags& flags,
                                 const std::function<void(uint8_t)>& displayCallback) const = 0;
};

#endif /* NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_ILAYOUTMODIFIERSTRATEGY_HPP_ */
