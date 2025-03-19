/*
 * KeyProtocolManager.hpp
 *
 *  Created on: Sep 19, 2018
 *      Author: Labo
 */

#ifndef KEYPROTOCOLMANAGER_KEYPROTOCOLMANAGER_HPP_
#define KEYPROTOCOLMANAGER_KEYPROTOCOLMANAGER_HPP_

#include "IKeepAliveListener.hpp"
#include "BLEPairing/IBLEPairingListener.hpp"
#include "AbstractRecursiveMutex.hpp"
#include "KeyProtocolDefs.hpp"
#include <vector>
#include "IKeyProtocolListener.hpp"

class KeyProtocolManager : public IKeepAliveListener, public IBLEPairingListener
{
public:
    explicit KeyProtocolManager(AbstractRecursiveMutex& mutex);
    virtual ~KeyProtocolManager() = default;

    void registerKeyProtocolListener(IKeyProtocolListener& keyProtocolListener);

    // Events
    void init();
    void keepAlive() override;
    void keepAliveTimeout() override;
    bool setProtocolMode(keyprotocoldefs::KeyProtocolMode protocolMode);

    void pairingStart(const BLEDefs::Passkey& passkey) override;
    void pairingStop() override;

private:
    enum class KeyProtocolState
    {
        INIT,
        NOCONN_HID,
        CONN_CUSTOM,
        CONN_HID,
        PAIRING
    };

    AbstractRecursiveMutex& mMutex;

    KeyProtocolState mProtocolState = KeyProtocolState::INIT;

    std::vector<IKeyProtocolListener*> mKeyProtocolListeners;

    void doSetCustomMode();
    void doSetHIDMode();
    void doSetPairingMode();
    void doSetNewMode(keyprotocoldefs::KeyProtocolMode mode);

    void notifyKeyProtocolListeners(keyprotocoldefs::KeyProtocolMode mode);
};

#endif /* KEYPROTOCOLMANAGER_KEYPROTOCOLMANAGER_HPP_ */
