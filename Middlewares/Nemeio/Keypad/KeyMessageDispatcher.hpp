/*
 * KeyMessageDispatcher.hpp
 *
 *  Created on: Dec 16, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_KEYPAD_KEYMESSAGEDISPATCHER_HPP_
#define NEMEIO_KEYPAD_KEYMESSAGEDISPATCHER_HPP_

#include "NotifyKeyHandlerStrategy.hpp"
#include "NoneNotifyKeyHandlerStrategy.hpp"
#include "IConfChangeListener.hpp"
#include "../NemeioKeypad/iNemeioKeypadPressedListener/iNemeioKeypadPressedListener.h"
#include "IKeyProtocolListener.hpp"
#include "AbstractRecursiveMutex.hpp"
#include "etl/optional.h"

class KeyMessageDispatcher : public iNemeioKeypadPressedListener,
                             public IConfChangeListener,
                             public IKeyProtocolListener
{
public:
    explicit KeyMessageDispatcher(AbstractRecursiveMutex& mutex);
    virtual ~KeyMessageDispatcher() = default;

    void registerHIDKeyMessageHandler(IKeyMessageHandler& messageHandler);
    void registerCustomProtocolKeyMessageHandler(IKeyMessageHandler& messageHandler);
    void registerPairingKeyMessageHandler(IKeyMessageHandler& messageHandler);

    void pressedKey(const ButtonID& button) override;
    void unpressedKey(const ButtonID& button) override;
    void terminateUpdateKeys() override;

    void configurationChanged(const Configuration* pConfig) override;

    void keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode keyProtocolMode) override;

private:
    AbstractRecursiveMutex& mMutex;

    NotifyKeyHandlerStrategy<IKeyMessageHandler> mCustomProtocolNotifyKeyHandlerStrategy;
    NotifyKeyHandlerStrategy<IKeyMessageHandler> mHIDNotifyKeyHandlerStrategy;
    NotifyKeyHandlerStrategy<IKeyMessageHandler> mPairingNotifyKeyHandlerStrategy;
    NoneNotifyKeyHandlerStrategy mNoneNotifyKeyHandlerStrategy;
    INotifyKeyHandlerStrategy* mpNotifyKeyHandlerStrategy = &mNoneNotifyKeyHandlerStrategy;

    etl::optional<Configuration> mConfig;

    keyprotocoldefs::KeyProtocolMode mKeyProtocolMode = keyprotocoldefs::KeyProtocolMode::UNDEFINED;

    void keypadActionsChanged();
    void resetKeyMessageHandlers();

    template<typename NotifyFunctor>
    void doNotifyKeyMessageHandlers(NotifyFunctor notify) const;

    void updateStrategy(keyprotocoldefs::KeyProtocolMode mode);
};

#endif /* NEMEIO_KEYPAD_KEYMESSAGEDISPATCHER_HPP_ */
