/*
 * KeyboardDisplayGlobalStatus.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_KEYBOARDDISPLAYGLOBALSTATUS_HPP_
#define DISPLAY_KEYBOARDDISPLAYGLOBALSTATUS_HPP_

#include "UpgradeDisplayState.hpp"
#include "InitDisplayStatus.hpp"
#include "DeinitDisplayState.hpp"
#include "MainDisplayStatus.hpp"
#include "KeyboardDisplayStatus.hpp"
#include "ModifiersDisplayState.hpp"
#include "ModifiersDisplayStatus.hpp"
#include "ApplicationConfig.h"
#include "AutoLock.h"

template<std::size_t COMPRESSED_FILE_SIZE>
class KeyboardDisplayGlobalStatusBase
{
public:
    explicit KeyboardDisplayGlobalStatusBase(StreamDataReadTask& streamDataReadTask);
    virtual ~KeyboardDisplayGlobalStatusBase() = default;

    void update(IScreen& screen);

    void postInit();
    void postPowerOff(DeinitDisplayStateListener& listener);
    void postClearCompleteScreen();
    void postDrawLayout(const configuration::Layout& layout,
                        bool highQualityModifier,
                        configuration::OperatingSystem os);
    void postPairingStart(const BLEDefs::Passkey& passkey);
    void postPairingStop();
    void postModifiersState(LayoutModifiersDefs::LayoutModifierFlags flags);
    void postUpgrade(Signal& upgradeSignal);

private:
    RecursiveMutex mMutex;
    KeyboardDisplayStatus<UpgradeDisplayState<COMPRESSED_FILE_SIZE>> mUpgradeDisplayStatus;
    InitDisplayStatus mInitDisplayStatus;
    KeyboardDisplayStatus<DeinitDisplayState> mDeinitDisplayStatus;
    MainDisplayStatus<COMPRESSED_FILE_SIZE> mMainDisplayStatus;
    ModifiersDisplayStatus mModifiersDisplayStatus;

    ConfigurationDisplayInfo mLastConfigurationDisplayInfo{etl::nullopt,
                                                           configuration::OperatingSystem::UNDEFINED};
    bool mHighQualityModifier = false;
    uint8_t mUpgradeCounter = 0;

    std::array<uint8_t, COMPRESSED_FILE_SIZE> mCompressedReadFileBuffer;
    StreamDataReadTask& mStreamDataReadTask;

    void prepareTargetStateCopy();
    void resetCompleteDisplayAfterUpgrade();
    void resetAdditionalDisplay();
};

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::update(IScreen& screen)
{
    uint8_t fbNum = 0;

    prepareTargetStateCopy();

    mInitDisplayStatus.update(screen, fbNum);
    if(mInitDisplayStatus.isInitialized())
    {
        bool bDisplayUpdated = false;

        if(mUpgradeDisplayStatus.update(screen, fbNum))
        {
            resetCompleteDisplayAfterUpgrade();
        }

        bDisplayUpdated = mMainDisplayStatus.update(screen, fbNum);
        if(bDisplayUpdated)
        {
            resetAdditionalDisplay();
        }

        mModifiersDisplayStatus.setModifierStrategy(
            mMainDisplayStatus.getCurrentStateModifierStrategy());
        bDisplayUpdated = mModifiersDisplayStatus.update(screen, fbNum);
    }

    mDeinitDisplayStatus.update(screen, fbNum);
}

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::postClearCompleteScreen()
{
    ClearDisplayVariant clearDisplayVariant;
    MainDisplayState<COMPRESSED_FILE_SIZE> mainDisplayState(clearDisplayVariant);

    AutoLock autoLock(mMutex);
    mMainDisplayStatus.setTargetState(mainDisplayState);
}

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::postDrawLayout(
    const configuration::Layout& layout, bool highQualityModifier, configuration::OperatingSystem os)
{
    AutoLock autoLock(mMutex);

    mLastConfigurationDisplayInfo = ConfigurationDisplayInfo{layout, os};
    mHighQualityModifier = highQualityModifier;

    ConfigurationDisplayVariant configurationDisplayVariant(mLastConfigurationDisplayInfo,
                                                            mStreamDataReadTask,
                                                            mCompressedReadFileBuffer);
    MainDisplayState<COMPRESSED_FILE_SIZE> mainDisplayState(configurationDisplayVariant);

    mMainDisplayStatus.setTargetState(mainDisplayState);
}

template<std::size_t COMPRESSED_FILE_SIZE>
KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::KeyboardDisplayGlobalStatusBase(
    StreamDataReadTask& streamDataReadTask)
    : mStreamDataReadTask(streamDataReadTask)
{
}

template<std::size_t COMPRESSED_FILE_SIZE>
inline void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::postModifiersState(
    LayoutModifiersDefs::LayoutModifierFlags flags)
{
    ModifiersDisplayState modifiersDisplayState(mLastConfigurationDisplayInfo,
                                                flags,
                                                mHighQualityModifier);

    AutoLock autoLock(mMutex);
    mModifiersDisplayStatus.setTargetState(modifiersDisplayState);
}

template<std::size_t COMPRESSED_FILE_SIZE>
inline void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::resetCompleteDisplayAfterUpgrade()
{
    DeinitDisplayState deinitDisplayState;
    MainDisplayState<COMPRESSED_FILE_SIZE> mainDisplayState;

    AutoLock autoLock(mMutex);

    mDeinitDisplayStatus.setCurrentState(deinitDisplayState);
    mMainDisplayStatus.setCurrentState(mainDisplayState);

    resetAdditionalDisplay();
}

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::postUpgrade(Signal& upgradeSignal)
{
    AutoLock autoLock(mMutex);

    // It is ok if this counter overflows, as current and target state will still be different
    ++mUpgradeCounter;

    UpgradeDisplayState<COMPRESSED_FILE_SIZE> upgradeDisplayState(mUpgradeCounter, upgradeSignal);

    mUpgradeDisplayStatus.setTargetState(upgradeDisplayState);
}

template<std::size_t COMPRESSED_FILE_SIZE>
inline void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::prepareTargetStateCopy()
{
    AutoLock autoLock(mMutex);

    mInitDisplayStatus.prepareTargetStateCopy();
    mUpgradeDisplayStatus.prepareTargetStateCopy();
    mMainDisplayStatus.prepareTargetStateCopy();
    mModifiersDisplayStatus.prepareTargetStateCopy();
    mDeinitDisplayStatus.prepareTargetStateCopy();
}

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::resetAdditionalDisplay()
{
    ModifiersDisplayState modifiersDisplayState;
    mModifiersDisplayStatus.setCurrentState(modifiersDisplayState);
}

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::postInit()
{
    InitDisplayState initDisplayState(InitDisplayState::InitDisplayStateValue::INITIALIZED);

    AutoLock autoLock(mMutex);
    mInitDisplayStatus.setTargetState(initDisplayState);
}

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::postPowerOff(
    DeinitDisplayStateListener& listener)
{
    DeinitDisplayState deinitDisplayState(listener,
                                          DeinitDisplayState::DeinitDisplayStateValue::DEINITIALIZED);

    AutoLock autoLock(mMutex);
    mDeinitDisplayStatus.setTargetState(deinitDisplayState);
}

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::postPairingStart(
    const BLEDefs::Passkey& passkey)
{
    PairingDisplayVariant pairingDisplayVariant(passkey);
    MainDisplayState<COMPRESSED_FILE_SIZE> mainDisplayState(pairingDisplayVariant);

    AutoLock autoLock(mMutex);
    mMainDisplayStatus.setTargetState(mainDisplayState);
}

template<std::size_t COMPRESSED_FILE_SIZE>
void KeyboardDisplayGlobalStatusBase<COMPRESSED_FILE_SIZE>::postPairingStop()
{
    AutoLock autoLock(mMutex);

    ConfigurationDisplayVariant configurationDisplayVariant(mLastConfigurationDisplayInfo,
                                                            mStreamDataReadTask,
                                                            mCompressedReadFileBuffer);
    MainDisplayState<COMPRESSED_FILE_SIZE> mainDisplayState(configurationDisplayVariant);

    mMainDisplayStatus.setTargetState(mainDisplayState);
}

using KeyboardDisplayGlobalStatus = KeyboardDisplayGlobalStatusBase<MAX_DISPLAY_COMPRESSED_FILE_SIZE>;

#endif /* DISPLAY_KEYBOARDDISPLAYGLOBALSTATUS_HPP_ */
