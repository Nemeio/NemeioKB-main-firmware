/*
 * IKeyLockStatesStrategy.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

#ifndef HID_KEYLOCKSTATES_ABSTRACTKEYLOCKSTATESSTRATEGY_HPP_
#define HID_KEYLOCKSTATES_ABSTRACTKEYLOCKSTATESSTRATEGY_HPP_

#include "IKeyLockStatesListener.hpp"
#include "Identifiable.hpp"
#include "ListenerSubject/ListenerCollection.hpp"

template<std::size_t NB_LISTENERS>
class AbstractKeyLockStatesStrategy
{
public:
    AbstractKeyLockStatesStrategy(
        const Identifiable& source,
        ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& listeners);
    virtual ~AbstractKeyLockStatesStrategy() = default;

    virtual void init() = 0;
    virtual void deinit() = 0;
    virtual void bleDisconnected() = 0;
    void keyLockStatesChanged(const Identifiable& source,
                              const HIDDefs::KeyLockStates& keyLockStates);

protected:
    void resetKeyLockStates();

private:
    const Identifiable& mSource;
    ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& mListeners;
};

#include "AbstractKeyLockStatesStrategy.tpp"

#endif /* HID_KEYLOCKSTATES_ABSTRACTKEYLOCKSTATESSTRATEGY_HPP_ */
