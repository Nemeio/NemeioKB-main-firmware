/*
 * NoneNotifyKeyHandlerStrategy.hpp
 *
 *  Created on: Apr 16, 2019
 *      Author: Labo
 */

#ifndef KEYPROTOCOLMANAGER_NONENOTIFYKEYHANDLERSTRATEGY_HPP_
#define KEYPROTOCOLMANAGER_NONENOTIFYKEYHANDLERSTRATEGY_HPP_

#include "INotifyKeyHandlerStrategy.hpp"

class NoneNotifyKeyHandlerStrategy : public INotifyKeyHandlerStrategy
{
public:
    NoneNotifyKeyHandlerStrategy() = default;
    virtual ~NoneNotifyKeyHandlerStrategy() = default;

    void doNotify(const std::function<void(IKeyMessageHandler&, const Configuration&)>& notify,
                  const Configuration& config) override;
};

inline void NoneNotifyKeyHandlerStrategy::doNotify(
    const std::function<void(IKeyMessageHandler&, const Configuration&)>& notify,
    const Configuration& config)
{
    // Nothing to do
}

#endif /* KEYPROTOCOLMANAGER_NONENOTIFYKEYHANDLERSTRATEGY_HPP_ */
