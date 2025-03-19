/*
 * KeyProtocolManagerAssembly.hpp
 *
 *  Created on: Jan 5, 2021
 *      Author: eskoric
 */

#ifndef KEYPROTOCOLMANAGER_KEYPROTOCOLMANAGERASSEMBLY_HPP_
#define KEYPROTOCOLMANAGER_KEYPROTOCOLMANAGERASSEMBLY_HPP_

#include "RecursiveMutex.hpp"
#include "KeyProtocolManager.hpp"

class KeyProtocolManagerAssembly
{
public:
    KeyProtocolManagerAssembly();
    virtual ~KeyProtocolManagerAssembly() = default;

    [[nodiscard]] KeyProtocolManager& getKeyProtocolManagerItem();

private:
    RecursiveMutex mMutex;
    KeyProtocolManager mKeyProtocolManager;
};

inline KeyProtocolManagerAssembly::KeyProtocolManagerAssembly()
    : mKeyProtocolManager(mMutex)
{
}

inline KeyProtocolManager& KeyProtocolManagerAssembly::getKeyProtocolManagerItem()
{
    return mKeyProtocolManager;
}

#endif /* KEYPROTOCOLMANAGER_KEYPROTOCOLMANAGERASSEMBLY_HPP_ */
