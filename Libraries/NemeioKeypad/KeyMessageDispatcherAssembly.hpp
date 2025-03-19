/*
 * KeyMessageDispatcherAssembly.hpp
 *
 *  Created on: Jan 5, 2021
 *      Author: eskoric
 */

#ifndef NEMEIOKEYPAD_KEYMESSAGEDISPATCHERASSEMBLY_HPP_
#define NEMEIOKEYPAD_KEYMESSAGEDISPATCHERASSEMBLY_HPP_

#include "RecursiveMutex.hpp"
#include "KeyMessageDispatcher.hpp"

class KeyMessageDispatcherAssembly
{
public:
    KeyMessageDispatcherAssembly();
    virtual ~KeyMessageDispatcherAssembly() = default;

    [[nodiscard]] KeyMessageDispatcher& getKeyMessageDispatcherItem();

private:
    RecursiveMutex mMutex;
    KeyMessageDispatcher mKeyMessageDispatcher;
};

inline KeyMessageDispatcherAssembly::KeyMessageDispatcherAssembly()
    : mKeyMessageDispatcher(mMutex)
{
}

inline KeyMessageDispatcher& KeyMessageDispatcherAssembly::getKeyMessageDispatcherItem()
{
    return mKeyMessageDispatcher;
}

#endif /* NEMEIOKEYPAD_KEYMESSAGEDISPATCHERASSEMBLY_HPP_ */
