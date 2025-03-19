/*
 * IKeyMessageHandler.hpp
 *
 *  Created on: Jul 10, 2019
 *      Author: Labo
 */

#ifndef NEMEIOKEYPAD_IKEYMESSAGEHANDLER_HPP_
#define NEMEIOKEYPAD_IKEYMESSAGEHANDLER_HPP_

#include "Configuration.hpp"

class IKeyMessageHandler
{
public:
    IKeyMessageHandler() = default;
    virtual ~IKeyMessageHandler() = default;

    virtual void pressedKeyWithConfig(const ButtonID& button, const Configuration& config) = 0;
    virtual void unpressedKeyWithConfig(const ButtonID& button, const Configuration& config) = 0;
    virtual void terminateUpdateKeysWithConfig(const Configuration& config) = 0;
    virtual void resetKeysWithConfig(const Configuration& config) = 0;
};

#endif /* NEMEIOKEYPAD_IKEYMESSAGEHANDLER_HPP_ */
