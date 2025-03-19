/*
 * CustomKeyProtocolMessageHandler.hpp
 *
 *  Created on: Sep 14, 2018
 *      Author: Labo
 */

#ifndef NEMEIO_NEMEIOKEYPAD_COMPUTERHIDMESSAGEHANDLER_CUSTOMKEYPROTOCOLMESSAGEHANDLER_HPP_
#define NEMEIO_NEMEIOKEYPAD_COMPUTERHIDMESSAGEHANDLER_CUSTOMKEYPROTOCOLMESSAGEHANDLER_HPP_

#include "IKeyMessageHandler.hpp"
#include "CustomKeyProtocol.hpp"

class CustomKeyProtocolMessageHandler : public IKeyMessageHandler
{
public:
    CustomKeyProtocolMessageHandler() = default;
    virtual ~CustomKeyProtocolMessageHandler() = default;

    void registerCustomKeyProtocol(CustomKeyProtocol& customKeyProtocol);
    void unregisterCustomKeyProtocol(CustomKeyProtocol& customKeyProtocol);

    void pressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void unpressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void terminateUpdateKeysWithConfig(const Configuration& config) final;
    void resetKeysWithConfig(const Configuration& config) final;

private:
    std::vector<CustomKeyProtocol*> mCustomKeyProtocols;

    void selectTouchForSenders(const ButtonID& button, bool bPressed, const Configuration& config);
};

#endif /* NEMEIO_NEMEIOKEYPAD_COMPUTERHIDMESSAGEHANDLER_CUSTOMKEYPROTOCOLMESSAGEHANDLER_HPP_ */
