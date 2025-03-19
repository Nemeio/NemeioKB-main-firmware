/*
 * ScreenProxy.hpp
 *
 *  Created on: Sep 19, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYER_HPP_
#define DISPLAY_DISPLAYER_HPP_

#include "fs.hpp"
#include "IDisplayer.hpp"
#include "FreeRTOS.h"
#include "DisplayState/KeyboardDisplayGlobalStatus.hpp"
#include "BLEPairing/IBLEPairingListener.hpp"
#include "DisplayState/DeinitDisplayStateListener.hpp"
#include "ILayoutModifierListener.hpp"
#include "IUpdateListener.hpp"
#include "Layout.hpp"

class Displayer : public IDisplayer,
                  public ILayoutModifierListener,
                  public IBLEPairingListener,
                  public DeinitDisplayStateListener
{
public:
    explicit Displayer(StreamDataReadTask& streamDataReadTask);
    virtual ~Displayer() = default;

    void init(IScreen& screen);
    void powerOff();
    void layoutModifiersStateChanged(const LayoutModifiersDefs::LayoutModifierFlags& flags);
    void clearCompleteScreen();
    void drawLayout(const configuration::Layout& layout,
                    bool highQualityModifier,
                    configuration::OperatingSystem os);
    void pairingStart(const BLEDefs::Passkey& passkey);
    void pairingStop();
    void updateStart();
    void updateStop();

private:
    static constexpr UBaseType_t KEYBOARD_DRAWER_TASK_PRIORITY = 2;
    static constexpr configSTACK_DEPTH_TYPE KEYBOARD_DRAWER_TASK_STACK_SIZE = 768;

    etl::optional<etl::reference_wrapper<IScreen>> mScreen = etl::nullopt;
    Signal mDrawSignal;
    Signal mPowerOffSignal;
    Signal mUgradeSignal;
    DelayMs mDelay;
    KeyboardDisplayGlobalStatus mKeyboardDisplayStatus;

    bool mbCapsLockState;

    TaskHandle_t mHandle;

    void run();

    static void vKeyboardDrawerTask(void* pvParameters);
    void drawCurrentState();
    void performUpgrade();

    void postInit();
    void postPowerOff();
    void postClearCompleteScreen();
    void postDrawLayout(const configuration::Layout& layout,
                        bool highQualityModifier,
                        configuration::OperatingSystem os);
    void postModifiersState(const LayoutModifiersDefs::LayoutModifierFlags& flags);
    void postPairingStart(const BLEDefs::Passkey& passkey);
    void postPairingStop();

    void displayDeinited();
};

#endif /* DISPLAY_DISPLAYER_HPP_ */
