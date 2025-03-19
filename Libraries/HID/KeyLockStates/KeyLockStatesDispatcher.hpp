/*
 * KeyLockStatesDisplay.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

#ifndef HID_KEYLOCKSTATES_KEYLOCKSTATESDISPATCHER_HPP_
#define HID_KEYLOCKSTATES_KEYLOCKSTATESDISPATCHER_HPP_

#include "IKeyLockStatesListener.hpp"
#include "ConnectivityStrategySwitcher.hpp"
#include "BleEventObserver.hpp"
#include "BLEKeyLockStatesStrategy.hpp"
#include "USBKeyLockStatesStrategy.hpp"
#include "NoneKeyLockStatesStrategy.hpp"

template<std::size_t NB_LISTENERS>
class KeyLockStatesDispatcher
    : public ConnectivityStrategySwitcher<AbstractKeyLockStatesStrategy<NB_LISTENERS>>,
      public BleEventObserver,
      public IKeyLockStatesListener
{
public:
    KeyLockStatesDispatcher(Identifiable& usbHIDSource,
                            Identifiable& bleHIDSource,
                            std::array<IKeyLockStatesListener*, NB_LISTENERS> listeners);
    virtual ~KeyLockStatesDispatcher() = default;

    void notification(BleEvent event) final;

    void keyLockStatesChanged(const Identifiable& source,
                              const HIDDefs::KeyLockStates& keyLockStates);

private:
    ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS> mListeners;
    USBKeyLockStatesStrategy<NB_LISTENERS> mUSBKeyLockStatesStrategy;
    BLEKeyLockStatesStrategy<NB_LISTENERS> mBLEKeyLockStatesStrategy;

    NoneKeyLockStatesStrategy<NB_LISTENERS> mNoneKeyLockStatesStrategy;
};

#include "KeyLockStatesDispatcher.tpp"

#endif /* HID_KEYLOCKSTATES_KEYLOCKSTATESDISPATCHER_HPP_ */
