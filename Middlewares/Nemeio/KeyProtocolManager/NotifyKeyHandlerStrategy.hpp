/*
 * NotifyKeyHandlerStrategy.hpp
 *
 *  Created on: Apr 16, 2019
 *      Author: Labo
 */

#ifndef KEYPROTOCOLMANAGER_NOTIFYKEYHANDLERSTRATEGY_HPP_
#define KEYPROTOCOLMANAGER_NOTIFYKEYHANDLERSTRATEGY_HPP_

#include <INotifyKeyHandlerStrategy.hpp>
#include <vector>

template<typename KeyHandler>
class NotifyKeyHandlerStrategy : public INotifyKeyHandlerStrategy
{
public:
    NotifyKeyHandlerStrategy() = default;
    virtual ~NotifyKeyHandlerStrategy() = default;

    void doNotify(const std::function<void(IKeyMessageHandler&, const Configuration&)>& notify,
                  const Configuration& config) override;

    void registerKeyHandler(KeyHandler& keyHandler);

private:
    std::vector<KeyHandler*> mKeyHandlers;
};

template<typename KeyHandler>
inline void NotifyKeyHandlerStrategy<KeyHandler>::doNotify(
    const std::function<void(IKeyMessageHandler&, const Configuration&)>& notify,
    const Configuration& config)
{
    if(!notify)
    {
        return;
    }

    for(auto ite = mKeyHandlers.begin(); ite != mKeyHandlers.end(); ++ite)
    {
        if(*ite != nullptr)
        {
            notify(**ite, config);
        }
    }
}

template<typename KeyHandler>
inline void NotifyKeyHandlerStrategy<KeyHandler>::registerKeyHandler(KeyHandler& keyHandler)
{
    mKeyHandlers.push_back(&keyHandler);
}

#endif /* KEYPROTOCOLMANAGER_NOTIFYKEYHANDLERSTRATEGY_HPP_ */
