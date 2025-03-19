/*
 * NoLayoutModifierStrategy.hpp
 *
 *  Created on: 2 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_NOLAYOUTMODIFIERSTRATEGY_HPP_
#define NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_NOLAYOUTMODIFIERSTRATEGY_HPP_

#include "AbstractLayoutModifierStrategy.hpp"

class NoLayoutModifierStrategy : public AbstractLayoutModifierStrategy
{
public:
    explicit NoLayoutModifierStrategy();
    virtual ~NoLayoutModifierStrategy() = default;

    LayoutModifiersDefs::FrameBufferIndex convertFlagsToFbNum(
        const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    void updateModifiers(const LayoutModifiersDefs::LayoutModifierFlags& flags,
                         const std::function<void(uint8_t)>& displayCallback) const;

    uint8_t getNbImages() const;

private:
    static constexpr uint8_t NB_MANAGED_COMBINATIONS =
        LayoutModifiersDefs::NB_IMAGES_CONFIGURATION_MODIFIERS_DISABLED;
};

inline LayoutModifiersDefs::FrameBufferIndex NoLayoutModifierStrategy::convertFlagsToFbNum(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return LayoutModifiersDefs::FrameBufferIndex::DEFAULT;
}

inline void NoLayoutModifierStrategy::updateModifiers(
    const LayoutModifiersDefs::LayoutModifierFlags& flags,
    const std::function<void(uint8_t)>& displayCallback) const
{
    // Nothing to display
}

inline uint8_t NoLayoutModifierStrategy::getNbImages() const
{
    return NB_MANAGED_COMBINATIONS;
}

inline NoLayoutModifierStrategy::NoLayoutModifierStrategy()
    : AbstractLayoutModifierStrategy()
{
}


#endif /* NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_NOLAYOUTMODIFIERSTRATEGY_HPP_ */
