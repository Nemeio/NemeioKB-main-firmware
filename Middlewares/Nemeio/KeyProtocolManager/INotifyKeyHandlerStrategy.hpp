/*
 * INotifyKeyHandlerStrategy.hpp
 *
 *  Created on: Apr 16, 2019
 *      Author: Labo
 */

#ifndef KEYPROTOCOLMANAGER_INOTIFYKEYHANDLERSTRATEGY_HPP_
#define KEYPROTOCOLMANAGER_INOTIFYKEYHANDLERSTRATEGY_HPP_

#include "IKeyMessageHandler.hpp"
#include "Configuration.hpp"
#include <functional>

class INotifyKeyHandlerStrategy
{
public:
    INotifyKeyHandlerStrategy() = default;
    virtual ~INotifyKeyHandlerStrategy() = default;

    virtual void doNotify(
        const std::function<void(IKeyMessageHandler&, const Configuration&)>& notify,
        const Configuration& config) = 0;
};

#endif /* KEYPROTOCOLMANAGER_INOTIFYKEYHANDLERSTRATEGY_HPP_ */
