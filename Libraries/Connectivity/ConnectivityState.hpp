/*
 * ConnectivityState.hpp
 *
 *  Created on: Oct 10, 2019
 *      Author: Labo
 */

#ifndef CONNECTIVITY_CONNECTIVITYSTATE_HPP_
#define CONNECTIVITY_CONNECTIVITYSTATE_HPP_

#include "AbstractConnectivityState.hpp"
#include <algorithm>

template<std::size_t N>
class ConnectivityState : public AbstractConnectivityState
{
public:
    ConnectivityState(std::array<IConnectivityListener*, N> listeners);
    virtual ~ConnectivityState() = default;

private:
    ListenerCollection<IConnectivityListener*, N> mListeners;

    void notifyListeners(std::function<void(IConnectivityListener&)> notify);
};

template<std::size_t N>
ConnectivityState<N>::ConnectivityState(std::array<IConnectivityListener*, N> listeners)
    : AbstractConnectivityState()
    , mListeners(listeners)
{
}

template<std::size_t N>
void ConnectivityState<N>::notifyListeners(std::function<void(IConnectivityListener&)> notify)
{
    mListeners.notify(
        [notify](IConnectivityListener* listener)
        {
            if(listener != nullptr)
            {
                notify(*listener);
            }
        });
}

#endif /* CONNECTIVITY_CONNECTIVITYSTATE_HPP_ */
