/*
 * keyIDConfig.h
 *
 *  Created on: 17 mai 2017
 *      Author: cgarnier
 */

#ifndef KEYIDCONFIG_H_
#define KEYIDCONFIG_H_

#include "IGPIOController.hpp"
#include "GPIOPins.h"
#include <string.h>
#include <cstdint>
#include "ButtonID.hpp"

#define BTN_INVALID \
    { \
        0xff, 0xff \
    }

#define BTN_UP \
    { \
        9, 6 \
    }
#define BTN_DOWN \
    { \
        10, 6 \
    }
#define BTN_ESCAPE \
    { \
        0, 0 \
    }

#define BTN_SHIFT_L \
    { \
        4, 6 \
    }
#define BTN_SHIFT_R \
    { \
        5, 6 \
    }
#define BTN_CTRL_L_MAC_FN \
    { \
        6, 6 \
    }
#define BTN_FN_MAC_CTRL_L \
    { \
        1, 6 \
    }
#define BTN_GUI_L_MAC_ALT_L \
    { \
        2, 6 \
    }
#define BTN_ALT_L_MAC_GUI_L \
    { \
        0, 6 \
    }
#define BTN_ALT_R_MAC_GUI_R \
    { \
        8, 6 \
    }
#define BTN_CTRL_R_MAC_ALT_R \
    { \
        7, 6 \
    }

#define BTN_F1 \
    { \
        1, 0 \
    }

#define BTN_F2 \
    { \
        2, 0 \
    }

#define BTN_F3 \
    { \
        3, 0 \
    }

#define BTN_F4 \
    { \
        4, 0 \
    }
#define BTN_F5 \
    { \
        5, 0 \
    }

#define BTN_ENTER \
    { \
        3, 6 \
    }
#define BTN_BACKSPACE \
    { \
        13, 1 \
    }

typedef uint32_t KeyIntIndex;

typedef struct ButtonPinAssoc_t
{
    GPIOPin gpioPin;
    ButtonID buttonID;
} ButtonPinAssoc;

typedef struct StringButtonIDAssoc_t
{
    const char* detail;
    ButtonID buttonID;
} StringButtonIDAssoc;

#endif /* __KEYIDCONFIG_H_ */
