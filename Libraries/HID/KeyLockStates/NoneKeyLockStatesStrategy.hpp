/*
 * NoneKeyLockStatesStrategy.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

#ifndef HID_KEYLOCKSTATES_NONEKEYLOCKSTATESSTRATEGY_HPP_
#define HID_KEYLOCKSTATES_NONEKEYLOCKSTATESSTRATEGY_HPP_

#include "AbstractKeyLockStatesStrategy.hpp"

template<std::size_t NB_LISTENERS>
class NoneKeyLockStatesStrategy : public AbstractKeyLockStatesStrategy<NB_LISTENERS>
{
public:
    NoneKeyLockStatesStrategy(ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& listeners);
    virtual ~NoneKeyLockStatesStrategy() = default;

    void init();
    void deinit();
    void bleDisconnected();

private:
    Identifiable mDefaultSource;
};

template<std::size_t NB_LISTENERS>
inline NoneKeyLockStatesStrategy<NB_LISTENERS>::NoneKeyLockStatesStrategy(
    ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& listeners)
    : AbstractKeyLockStatesStrategy<NB_LISTENERS>(mDefaultSource, listeners)
{
}

template<std::size_t NB_LISTENERS>
inline void NoneKeyLockStatesStrategy<NB_LISTENERS>::init()
{
}

template<std::size_t NB_LISTENERS>
inline void NoneKeyLockStatesStrategy<NB_LISTENERS>::deinit()
{
}

template<std::size_t NB_LISTENERS>
inline void NoneKeyLockStatesStrategy<NB_LISTENERS>::bleDisconnected()
{
}

#endif /* HID_KEYLOCKSTATES_NONEKEYLOCKSTATESSTRATEGY_HPP_ */
