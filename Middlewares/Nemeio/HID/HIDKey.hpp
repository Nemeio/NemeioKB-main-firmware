/*
 * HIDKey.hpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_HID_HIDKEY_HPP_
#define NEMEIO_HID_HIDKEY_HPP_

#include "KeyboardContext.hpp"

class HIDKey
{
public:
    enum Keycode
    {
        null = 0,
        a = 0x04,
        b = 0x05,
        c = 0x06,
        d = 0x07,
        e = 0x08,
        f = 0x09,
        g = 0x0A,
        h = 0x0B,
        i = 0x0C,
        j = 0x0D,
        k = 0x0E,
        l = 0x0F,
        m = 0x10,
        n = 0x11,
        o = 0x12,
        p = 0x13,
        q = 0x14,
        r = 0x15,
        s = 0x16,
        t = 0x17,
        u = 0x18,
        v = 0x19,
        w = 0x1A,
        x = 0x1B,
        y = 0x1C,
        z = 0x1D,
        b_1 = 0x1E,
        b_2 = 0x1F,
        b_3 = 0x20,
        b_4 = 0x21,
        b_5 = 0x22,
        b_6 = 0x23,
        b_7 = 0x24,
        b_8 = 0x25,
        b_9 = 0x26,
        b_0 = 0x27,
        enter = 0x28,
        escape = 0x29,
        delete_bksp = 0x2A,
        tab = 0x2B,
        spacebar = 0x2C,
        minus = 0x2D,
        equal = 0x2E,
        bracket_in = 0x2F,
        bracket_out = 0x30,
        anti_slash = 0x31,
        non_us_sharp = 0x32,
        semicolon = 0x33,
        apostrophe = 0x34,
        grave_accent = 0x35,
        comma = 0x36,
        dot = 0x37,
        slash = 0x38,
        caps_lock = 0x39,
        F1 = 0x3A,
        F2 = 0x3B,
        F3 = 0x3C,
        F4 = 0x3D,
        F5 = 0x3E,
        F6 = 0x3F,
        F7 = 0x40,
        F8 = 0x41,
        F9 = 0x42,
        F10 = 0x43,
        F11 = 0x44,
        F12 = 0x45,
        print_screen = 0x46,
        scroll_lock = 0x47,
        pause = 0x48,
        insert = 0x49,
        home_kc = 0x4A,
        page_up = 0x4B,
        delete_forward = 0x4C,
        end = 0x4D,
        page_down = 0x4E,
        right_arrow = 0x4F,
        left_arrow = 0x50,
        down_arrow = 0x51,
        up_arrow = 0x52,
        // Numerical keypad
        num_lock = 0x53,
        num_slash = 0x54,
        num_star = 0x55,
        num_minus = 0x56,
        num_plus = 0x57,
        num_enter = 0x58,
        num_1 = 0x59,
        num_2 = 0x5A,
        num_3 = 0x5B,
        num_4 = 0x5C,
        num_5 = 0x5D,
        num_6 = 0x5E,
        num_7 = 0x5F,
        num_8 = 0x60,
        num_9 = 0x61,
        num_0 = 0x62,
        num_dot = 0x63,
        non_us_anti_slash = 0x64, // Near Left shift key in AT-102 implementation. Fr Kbd : <>
        app = 0x65,
        //modifiers need to be written 0xFx then will be converted to real value with &0x0F
        //otherwise there will be multiple declaration
        /*Explanation of modifier byte
			 *|   bit7  |   bit6  |    bit5   |   bit4   |  bit3  |  bit2  |   bit1   |   bit0  |
			 ************************************************************************************
			 *|Right GUI|Right Alt|Right Shift|Right ctrl|Left GUI|Left Alt|Left shift|Left Ctrl|
			 */
        ctrl_left = 0xFF01,   //0x01
        shift_left = 0xFF02,  //0x02,
        alt_left = 0xFF04,    //0x04,
        GUI_left = 0xFF08,    // 0x08,
        ctrl_right = 0xFF10,  //0x10
        shift_right = 0xFF20, //0x20,
        alt_right = 0xFF40,   //0x40,
        GUI_right = 0xFF80,   //0x80,
        Fn,
        back,
        forward,
        stop,
        refresh,
        home,
        email_reader,
        file_explorer,
        calculator,
        ctrl_config,
        volume_up,
        volume_down,
        volume_mute,
        track_play,
        track_pause,
        track_stop,
        track_play_pause,
        audio_browser,
        search,
        brightness_inc,
        brightness_dec,
        sleep,
    };

    explicit HIDKey(Keycode keycode);
    virtual ~HIDKey() = default;

    virtual Keycode pressedKey(const KeyboardContext& kbdContext);
    virtual Keycode unpressedKey();

protected:
    const Keycode mKeycode;
};

#endif /* NEMEIO_HID_HIDKEY_HPP_ */
