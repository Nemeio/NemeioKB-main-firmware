/*
 * KeypadModifierConverter.hpp
 *
 *  Created on: 29 mai 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_KEYPAD_KEYPADMODIFIERCONVERTER_HPP_
#define NEMEIO_KEYPAD_KEYPADMODIFIERCONVERTER_HPP_

#include "IKeyLockStatesListener.hpp"
#include "IKeyMessageHandler.hpp"
#include "ILayoutModifierListener.hpp"
#include <bitset>
#include "AbstractRecursiveMutex.hpp"

class LayoutModifiersState : public IKeyLockStatesListener, public IKeyMessageHandler
{
public:
    LayoutModifiersState(AbstractRecursiveMutex& mutex, ILayoutModifierListener& listener);
    virtual ~LayoutModifiersState() = default;

    void keyLockStatesChanged(const Identifiable& source,
                              const HIDDefs::KeyLockStates& keyLockStates);

    void pressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void unpressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void terminateUpdateKeysWithConfig(const Configuration& config) final;
    void resetKeysWithConfig(const Configuration& config) final;

private:
    LayoutModifiersDefs::LayoutModifierFlags mState;
    AbstractRecursiveMutex& mStateMutex;
    ILayoutModifierListener& mListener;

    void selectTouch(const ButtonID& button, bool bPressed, const Configuration& config);
    void notifyState(const LayoutModifiersDefs::LayoutModifierFlags& state);
    void updateAndNotifyState(LayoutModifiersDefs::LayoutModifierFlags flags, bool bPressed);
};

#endif /* NEMEIO_KEYPAD_KEYPADMODIFIERCONVERTER_HPP_ */
