/*
 * BLEKeyLockStatesStrategy.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

#ifndef HID_KEYLOCKSTATES_BLEKEYLOCKSTATESSTRATEGY_HPP_
#define HID_KEYLOCKSTATES_BLEKEYLOCKSTATESSTRATEGY_HPP_

#include "AbstractKeyLockStatesStrategy.hpp"

template<std::size_t NB_LISTENERS>
class BLEKeyLockStatesStrategy : public AbstractKeyLockStatesStrategy<NB_LISTENERS>
{
public:
    BLEKeyLockStatesStrategy(const Identifiable& source,
                             ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& listeners);
    virtual ~BLEKeyLockStatesStrategy() = default;

    void init();
    void deinit();
    void bleDisconnected();
};

#include "BLEKeyLockStatesStrategy.tpp"

#endif /* HID_KEYLOCKSTATES_BLEKEYLOCKSTATESSTRATEGY_HPP_ */
