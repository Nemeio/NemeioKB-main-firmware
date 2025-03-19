/*
 * BLEKeyLockStatesStrategy.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

template<std::size_t NB_LISTENERS>
BLEKeyLockStatesStrategy<NB_LISTENERS>::BLEKeyLockStatesStrategy(
    const Identifiable& source, ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& listeners)
    : AbstractKeyLockStatesStrategy<NB_LISTENERS>(source, listeners)
{
}

template<std::size_t NB_LISTENERS>
void BLEKeyLockStatesStrategy<NB_LISTENERS>::init()
{
}

template<std::size_t NB_LISTENERS>
void BLEKeyLockStatesStrategy<NB_LISTENERS>::deinit()
{
}

template<std::size_t NB_LISTENERS>
void BLEKeyLockStatesStrategy<NB_LISTENERS>::bleDisconnected()
{
    this->resetKeyLockStates();
}
