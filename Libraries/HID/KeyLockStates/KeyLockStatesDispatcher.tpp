/*
 * KeyLockStatesDisplay.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

template<std::size_t NB_LISTENERS>
KeyLockStatesDispatcher<NB_LISTENERS>::KeyLockStatesDispatcher(
    Identifiable& usbHIDSource,
    Identifiable& bleHIDSource,
    std::array<IKeyLockStatesListener*, NB_LISTENERS> listeners)
    : ConnectivityStrategySwitcher<AbstractKeyLockStatesStrategy<NB_LISTENERS>>(
        mUSBKeyLockStatesStrategy, mBLEKeyLockStatesStrategy, mNoneKeyLockStatesStrategy)
    , mListeners(listeners)
    , mUSBKeyLockStatesStrategy(usbHIDSource, mListeners)
    , mBLEKeyLockStatesStrategy(bleHIDSource, mListeners)
    , mNoneKeyLockStatesStrategy(mListeners)
{
}

template<std::size_t NB_LISTENERS>
void KeyLockStatesDispatcher<NB_LISTENERS>::notification(BleEvent event)
{
    switch(event)
    {
    case BleEvent::DISCONNECTED:
        this->getCurrentStrategy().bleDisconnected();
        break;
    default:
        break;
    }
}

template<std::size_t NB_LISTENERS>
void KeyLockStatesDispatcher<NB_LISTENERS>::keyLockStatesChanged(
    const Identifiable& source, const HIDDefs::KeyLockStates& keyLockStates)
{
    this->getCurrentStrategy().keyLockStatesChanged(source, keyLockStates);
}
