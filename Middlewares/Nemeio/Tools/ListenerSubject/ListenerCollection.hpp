/*
 * ListenerSubject.hpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_TOOLS_LISTENERSUBJECT_LISTENERCOLLECTION_HPP_
#define NEMEIO_TOOLS_LISTENERSUBJECT_LISTENERCOLLECTION_HPP_

#include <cstddef>
#include <array>
#include "assertTools.h"
#include "IListenerCollection.hpp"

template<typename T, std::size_t NB_LISTENERS>
class ListenerCollection : public IListenerCollection<T>
{
public:
    explicit ListenerCollection(std::array<T, NB_LISTENERS> listeners);
    virtual ~ListenerCollection() = default;

    void notify(std::function<void(T)> notifyFunc);

private:
    std::array<T, NB_LISTENERS> mListeners;
};

template<typename T, std::size_t NB_LISTENERS>
inline ListenerCollection<T, NB_LISTENERS>::ListenerCollection(std::array<T, NB_LISTENERS> listeners)
{
    std::copy(listeners.begin(), listeners.end(), mListeners.begin());
}

template<typename T, std::size_t NB_LISTENERS>
inline void ListenerCollection<T, NB_LISTENERS>::notify(std::function<void(T)> notifyFunc)
{
    for(auto& listener: mListeners)
    {
        notifyFunc(listener);
    }
}

#endif /* NEMEIO_TOOLS_LISTENERSUBJECT_LISTENERCOLLECTION_HPP_ */
