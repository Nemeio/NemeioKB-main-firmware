/*
 * USBKeyLockStatesStrategy.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

template<std::size_t NB_LISTENERS>
USBKeyLockStatesStrategy<NB_LISTENERS>::USBKeyLockStatesStrategy(
    const Identifiable& source, ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& listeners)
    : AbstractKeyLockStatesStrategy<NB_LISTENERS>(source, listeners)
{
}

template<std::size_t NB_LISTENERS>
void USBKeyLockStatesStrategy<NB_LISTENERS>::init()
{
}

template<std::size_t NB_LISTENERS>
void USBKeyLockStatesStrategy<NB_LISTENERS>::deinit()
{
    this->resetKeyLockStates();
}

template<std::size_t NB_LISTENERS>
void USBKeyLockStatesStrategy<NB_LISTENERS>::bleDisconnected()
{
}
