/*
 * SysFailLogDemo.cpp
 *
 *  Created on: Jun 11, 2019
 *      Author: Labo
 */

#include "SysFailLogDemo.hpp"
#include "assertTools.h"

const ButtonID SysFailLogDemo::BUTTON_ASSERTFAIL = BTN_F4;
const ButtonID SysFailLogDemo::BUTTON_HARDFAULT = BTN_F5;

void SysFailLogDemo::pressedKey(const ButtonID& button)
{
    if(button.x == BUTTON_ASSERTFAIL.x && button.y == BUTTON_ASSERTFAIL.y)
    {
        volatile uint32_t testValue = 0xB1BB1D11;
        volatile uint32_t compTestValue = 0xBABD1B00;
        ASSERT(testValue == compTestValue);
    }
    else if(button.x == BUTTON_HARDFAULT.x && button.y == BUTTON_HARDFAULT.y)
    {
        void (*func)(void) = (void (*)(void)) 0x300;
        func();
    }
}

void SysFailLogDemo::unpressedKey(const ButtonID& button)
{
}

void SysFailLogDemo::terminateUpdateKeys()
{
}
