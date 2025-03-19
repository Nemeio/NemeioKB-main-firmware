/*
 * USBKeyLockStatesStrategy.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

#ifndef HID_KEYLOCKSTATES_USBKEYLOCKSTATESSTRATEGY_HPP_
#define HID_KEYLOCKSTATES_USBKEYLOCKSTATESSTRATEGY_HPP_

#include "AbstractKeyLockStatesStrategy.hpp"

template<std::size_t NB_LISTENERS>
class USBKeyLockStatesStrategy : public AbstractKeyLockStatesStrategy<NB_LISTENERS>
{
public:
    USBKeyLockStatesStrategy(const Identifiable& source,
                             ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& listeners);
    virtual ~USBKeyLockStatesStrategy() = default;

    void init();
    void deinit();
    void bleDisconnected();
};

#include "USBKeyLockStatesStrategy.tpp"

#endif /* HID_KEYLOCKSTATES_USBKEYLOCKSTATESSTRATEGY_HPP_ */
