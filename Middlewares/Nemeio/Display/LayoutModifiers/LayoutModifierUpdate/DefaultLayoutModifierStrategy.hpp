/*
 * DefaultLayoutModifierStrategy.hpp
 *
 *  Created on: 2 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_DEFAULTLAYOUTMODIFIERSTRATEGY_HPP_
#define NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_DEFAULTLAYOUTMODIFIERSTRATEGY_HPP_

#include "AbstractLayoutModifierStrategy.hpp"
#include "IScreen.hpp"

class DefaultLayoutModifierStrategy : public AbstractLayoutModifierStrategy
{
public:
    explicit DefaultLayoutModifierStrategy() = default;
    virtual ~DefaultLayoutModifierStrategy() = default;

    void updateModifiers(const LayoutModifiersDefs::LayoutModifierFlags& flags,
                         const std::function<void(uint8_t)>& displayCallback) const;
    LayoutModifiersDefs::FrameBufferIndex convertFlagsToFbNum(
        const LayoutModifiersDefs::LayoutModifierFlags flags) const;

    uint8_t getNbImages() const;

protected:
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt SHIFT_FLAGS =
        LayoutModifiersDefs::Flag::SHIFT_LEFT | LayoutModifiersDefs::Flag::SHIFT_RIGHT;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt ALT_RIGHT_FLAGS =
        LayoutModifiersDefs::Flag::ALT_RIGHT;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt ALT_LEFT_FLAGS =
        LayoutModifiersDefs::Flag::ALT_LEFT;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt CTRL_FLAGS =
        LayoutModifiersDefs::Flag::CTRL_LEFT | LayoutModifiersDefs::Flag::CTRL_RIGHT;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt GUI_FLAGS =
        LayoutModifiersDefs::Flag::GUI_LEFT | LayoutModifiersDefs::Flag::GUI_RIGHT;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt CAPSLOCK_FLAGS =
        LayoutModifiersDefs::Flag::CAPSLOCK;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt FN_FLAGS =
        LayoutModifiersDefs::Flag::FN;

    // To determine whether a combo is active or not, check that the right modifiers are active and that only the allowed ones
    // are set. For instance: AltGr and AltGr+Ctrl are equivalent, but Shift and Shit+Ctrl are not.
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt ALLOWED_SHIFT_COMBO_FLAGS =
        SHIFT_FLAGS;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt ALLOWED_ALT_GR_COMBO_FLAGS =
        ALT_RIGHT_FLAGS | ALT_LEFT_FLAGS | CTRL_FLAGS;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt ALLOWED_CAPSLOCK_COMBO_FLAGS =
        CAPSLOCK_FLAGS;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt ALLOWED_FN_COMBO_FLAGS =
        FN_FLAGS;

private:
    static constexpr uint8_t NB_MANAGED_COMBINATIONS =
        LayoutModifiersDefs::NB_IMAGES_CONFIGURATION_MODIFIERS_ENABLED;

    bool isShiftPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    bool isCtrlPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    virtual bool isAltPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    virtual bool isAltGrPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    bool isGuiPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    bool isCapslockPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    bool isFnPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;

    bool isShiftCombo(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    bool isAltGrCombo(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    virtual bool isShiftAltGrCombo(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    virtual bool isCapslockCombo(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    virtual bool isFnCombo(const LayoutModifiersDefs::LayoutModifierFlags flags) const;

    virtual LayoutModifiersDefs::LayoutModifierStateFlagsInt getAllowedAltGrComboFlags() const;
};

#endif /* NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_DEFAULTLAYOUTMODIFIERSTRATEGY_HPP_ */
