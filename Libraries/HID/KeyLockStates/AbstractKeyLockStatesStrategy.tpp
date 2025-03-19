/*
 * AbstractKeyLockStatesStrategy.cpp
 *
 *  Created on: Oct 14, 2019
 *      Author: Labo
 */

template<std::size_t NB_LISTENERS>
AbstractKeyLockStatesStrategy<NB_LISTENERS>::AbstractKeyLockStatesStrategy(
    const Identifiable& source, ListenerCollection<IKeyLockStatesListener*, NB_LISTENERS>& listeners)
    : mSource(source)
    , mListeners(listeners)
{
}

template<std::size_t NB_LISTENERS>
void AbstractKeyLockStatesStrategy<NB_LISTENERS>::keyLockStatesChanged(
    const Identifiable& source, const HIDDefs::KeyLockStates& keyLockStates)
{
    if(source == mSource)
    {
        mListeners.notify(
            [&](IKeyLockStatesListener* listener)
            {
                if(listener != nullptr)
                {
                    listener->keyLockStatesChanged(mSource, keyLockStates);
                }
            });
    }
}

template<std::size_t NB_LISTENERS>
void AbstractKeyLockStatesStrategy<NB_LISTENERS>::resetKeyLockStates()
{
    HIDDefs::KeyLockStates defaultKeyLockStates;
    mListeners.notify(
        [&](IKeyLockStatesListener* listener)
        {
            if(listener != nullptr)
            {
                listener->keyLockStatesChanged(mSource, defaultKeyLockStates);
            }
        });
}
