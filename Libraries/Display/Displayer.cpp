/*
 * ScreenProxy.cpp
 *
 *  Created on: Sep 19, 2019
 *      Author: Labo
 */

#include "Displayer.hpp"

#include "assertTools.h"
#include "utils.hpp"

#include "systemUtils.h"
#include "FreeRTOSUtils.hpp"

Displayer::Displayer(StreamDataReadTask& streamDataReadTask)
    : mDelay(DelayMs(DelayMs::INFINITE_DELAY))
    , mKeyboardDisplayStatus(streamDataReadTask)
    , mbCapsLockState(false)
{
    xTaskCreate(vKeyboardDrawerTask,
                "KeyboardDrawerTask",
                KEYBOARD_DRAWER_TASK_STACK_SIZE,
                this,
                KEYBOARD_DRAWER_TASK_PRIORITY,
                &mHandle);
}

void Displayer::postClearCompleteScreen()
{
    mKeyboardDisplayStatus.postClearCompleteScreen();
    mDrawSignal.set(0);
}

void Displayer::postDrawLayout(const configuration::Layout& layout,
                               bool highQualityModifier,
                               configuration::OperatingSystem os)
{
    mKeyboardDisplayStatus.postDrawLayout(layout, highQualityModifier, os);
    mDrawSignal.set(0);
}

void Displayer::vKeyboardDrawerTask(void* pvParameters)
{
    Displayer* keyboardDrawer = reinterpret_cast<Displayer*>(pvParameters);

    ASSERT(keyboardDrawer != NULL);

    for(;;)
    {
        keyboardDrawer->run();
    }
}

void Displayer::drawCurrentState()
{
    if(mScreen.has_value())
    {
        mKeyboardDisplayStatus.update(mScreen.value().get());
    }
}

void Displayer::run()
{
    if(mDrawSignal.wait(DelayMs(DelayMs::INFINITE_DELAY)))
    {
        drawCurrentState();
    }
}

void Displayer::clearCompleteScreen()
{
    postClearCompleteScreen();
}

void Displayer::drawLayout(const configuration::Layout& layout,
                           bool highQualityModifier,
                           configuration::OperatingSystem os)
{
    postDrawLayout(layout, highQualityModifier, os);
}

void Displayer::init(IScreen& screen)
{
    mScreen.emplace(screen);
    postInit();
}

void Displayer::powerOff()
{
    mPowerOffSignal.clear();
    postPowerOff();
    mPowerOffSignal.wait(DelayMs(DelayMs::INFINITE_DELAY));
}

void Displayer::postInit()
{
    mKeyboardDisplayStatus.postInit();
    mDrawSignal.set(0);
}

void Displayer::postPowerOff()
{
    mKeyboardDisplayStatus.postPowerOff(*this);
    mDrawSignal.set(0);
}

void Displayer::layoutModifiersStateChanged(const LayoutModifiersDefs::LayoutModifierFlags& flags)
{
    postModifiersState(flags);
}

void Displayer::pairingStart(const BLEDefs::Passkey& passkey)
{
    postPairingStart(passkey);
}

void Displayer::pairingStop()
{
    postPairingStop();
}

void Displayer::postPairingStart(const BLEDefs::Passkey& passkey)
{
    mKeyboardDisplayStatus.postPairingStart(passkey);
    mDrawSignal.set(0);
}

void Displayer::postPairingStop()
{
    mKeyboardDisplayStatus.postPairingStop();
    mDrawSignal.set(0);
}

void Displayer::postModifiersState(const LayoutModifiersDefs::LayoutModifierFlags& flags)
{
    mKeyboardDisplayStatus.postModifiersState(flags);
    mDrawSignal.set(0);
}

void Displayer::displayDeinited()
{
    mPowerOffSignal.set(0);
}

void Displayer::updateStart()
{
    if(mScreen.has_value())
    {
        mUgradeSignal.clear();
        mKeyboardDisplayStatus.postUpgrade(mUgradeSignal);
        mDrawSignal.set(0);
    }
}

void Displayer::updateStop()
{
    if(mScreen.has_value())
    {
        mUgradeSignal.set(0);
    }
}
