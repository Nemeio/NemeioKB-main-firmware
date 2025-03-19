/*
 * LayoutModifiersDefs.hpp
 *
 *  Created on: 29 mai 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_KEYPAD_LAYOUTMODIFIERCONVERTER_LAYOUTMODIFIERSDEFS_HPP_
#define NEMEIO_KEYPAD_LAYOUTMODIFIERCONVERTER_LAYOUTMODIFIERSDEFS_HPP_

#include <cstdint>
#include <cstddef>

namespace LayoutModifiersDefs
{
enum class Modifier : uint8_t
{
    CTRL_LEFT = 0,
    CTRL_RIGHT,
    ALT_LEFT,
    ALT_RIGHT,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    GUI_LEFT,
    GUI_RIGHT,
    FN,
    CAPSLOCK,
    MODIFIER_MAX
};

using LayoutModifierStateFlagsInt = uint32_t;
static_assert(static_cast<std::size_t>(Modifier::MODIFIER_MAX)
                  <= 8 * sizeof(LayoutModifierStateFlagsInt),
              "LayoutModifiersStateInt type is too small to contain all modifier flags.");

namespace Flag
{
constexpr LayoutModifierStateFlagsInt CTRL_LEFT = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                      Modifier::CTRL_LEFT);
constexpr LayoutModifierStateFlagsInt CTRL_RIGHT = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                       Modifier::CTRL_RIGHT);
constexpr LayoutModifierStateFlagsInt ALT_LEFT = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                     Modifier::ALT_LEFT);
constexpr LayoutModifierStateFlagsInt ALT_RIGHT = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                      Modifier::ALT_RIGHT);
constexpr LayoutModifierStateFlagsInt SHIFT_LEFT = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                       Modifier::SHIFT_LEFT);
constexpr LayoutModifierStateFlagsInt SHIFT_RIGHT = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                        Modifier::SHIFT_RIGHT);
constexpr LayoutModifierStateFlagsInt GUI_LEFT = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                     Modifier::GUI_LEFT);
constexpr LayoutModifierStateFlagsInt GUI_RIGHT = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                      Modifier::GUI_RIGHT);
constexpr LayoutModifierStateFlagsInt FN = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                               Modifier::FN);
constexpr LayoutModifierStateFlagsInt CAPSLOCK = 1 << static_cast<LayoutModifierStateFlagsInt>(
                                                     Modifier::CAPSLOCK);
} // namespace Flag

struct LayoutModifierFlags
{
    LayoutModifierStateFlagsInt flags;

    bool findAllFlags(LayoutModifierFlags modifierFlagMask) const
    {
        return (flags & modifierFlagMask.flags) == modifierFlagMask.flags;
    }

    bool findAsLeastOneFlag(LayoutModifierFlags modifierFlagMask) const
    {
        return (flags & modifierFlagMask.flags) != 0;
    }

    void set(LayoutModifierFlags modifierFlagMask, bool bValue)
    {
        if(bValue)
        {
            flags |= modifierFlagMask.flags;
        }
        else
        {
            flags &= ~modifierFlagMask.flags;
        }
    }

    bool operator==(const LayoutModifierFlags& compared) const { return compared.flags == flags; }

    bool operator!=(const LayoutModifierFlags& compared) const { return !(*this == compared); }

    constexpr LayoutModifierFlags()
        : flags(0)
    {
    }

    constexpr LayoutModifierFlags(LayoutModifierStateFlagsInt _flags)
        : flags(_flags)
    {
    }
};

inline constexpr uint8_t NB_IMAGES_CONFIGURATION_MODIFIERS_DISABLED = 1;
inline constexpr uint8_t NB_IMAGES_CONFIGURATION_MODIFIERS_ENABLED = 6;

enum class FrameBufferIndex
{
    DEFAULT = 0,
    CAPSLOCK,
    SHIFT,
    ALTGR,
    SHIFT_ALTGR,
    FN
};
} // namespace LayoutModifiersDefs


#endif /* NEMEIO_KEYPAD_LAYOUTMODIFIERCONVERTER_LAYOUTMODIFIERSDEFS_HPP_ */
