/*
 * IListenerCollection.hpp
 *
 *  Created on: 17 juil. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_TOOLS_LISTENERSUBJECT_ILISTENERCOLLECTION_HPP_
#define NEMEIO_TOOLS_LISTENERSUBJECT_ILISTENERCOLLECTION_HPP_

#include <functional>

template<typename T>
class IListenerCollection
{
public:
    IListenerCollection() = default;
    virtual ~IListenerCollection() = default;

    virtual void notify(std::function<void(T)> notifyFunc) = 0;
};

#endif /* NEMEIO_TOOLS_LISTENERSUBJECT_ILISTENERCOLLECTION_HPP_ */
