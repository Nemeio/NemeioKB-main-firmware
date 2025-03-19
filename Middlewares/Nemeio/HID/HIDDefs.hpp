/*
 * KeyLockStates.hpp
 *
 *  Created on: Sep 30, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_HID_HIDDEFS_HPP_
#define NEMEIO_HID_HIDDEFS_HPP_

#include <cstddef>
#include <cstdint>

namespace HIDDefs
{
inline constexpr std::size_t HID_SET_KEY_LOCK_STATES_LEN = 1;
inline constexpr uint8_t HID_SET_KEY_LOCK_STATES_NUMLOCK_MASK = (1 << 0);
inline constexpr uint8_t HID_SET_KEY_LOCK_STATES_CAPSLOCK_MASK = (1 << 1);
inline constexpr uint8_t HID_SET_KEY_LOCK_STATES_SCROLLLOCK_MASK = (1 << 2);
inline constexpr uint8_t HID_SET_KEY_LOCK_STATES_COMPOSE_MASK = (1 << 3);
inline constexpr uint8_t HID_SET_KEY_LOCK_STATES_KANA_MASK = (1 << 4);

inline constexpr uint8_t HID_KEY_LOCK_STATES_REPORT_ID = 0x02;

struct KeyLockStates
{
    bool numLock;
    bool capsLock;
    bool scrollLock;
    bool compose;
    bool kana;

    explicit KeyLockStates(uint8_t outputReport)
    {
        numLock = (outputReport & HID_SET_KEY_LOCK_STATES_NUMLOCK_MASK) != 0;
        capsLock = (outputReport & HID_SET_KEY_LOCK_STATES_CAPSLOCK_MASK) != 0;
        scrollLock = (outputReport & HID_SET_KEY_LOCK_STATES_SCROLLLOCK_MASK) != 0;
        compose = (outputReport & HID_SET_KEY_LOCK_STATES_COMPOSE_MASK) != 0;
        kana = (outputReport & HID_SET_KEY_LOCK_STATES_KANA_MASK) != 0;
    };

    KeyLockStates()
    {
        numLock = false;
        capsLock = false;
        scrollLock = false;
        compose = false;
        kana = false;
    };
};
} // namespace HIDDefs


#endif /* NEMEIO_HID_HIDDEFS_HPP_ */
