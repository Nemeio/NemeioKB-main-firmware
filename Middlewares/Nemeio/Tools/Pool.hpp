/*
 * Pool.hpp
 *
 *  Created on: 26 janv. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_TOOLS_POOL_HPP_
#define NEMEIO_TOOLS_POOL_HPP_

#include <array>
#include <cstddef>
#include "Mutex.hpp"
#include "AutoLock.h"

enum class ePoolObjectState
{
    FREE,
    BUSY
};

template<typename T>
class PoolObject
{
private:
    T mObject;
    ePoolObjectState mState;

public:
    PoolObject() { mState = ePoolObjectState::FREE; };
    virtual ~PoolObject() = default;
    [[nodiscard]] ePoolObjectState getState() const { return mState; };
    T* getObject() { return &mObject; };
    void allocateObject() { mState = ePoolObjectState::BUSY; };
    void freeObject() { mState = ePoolObjectState::FREE; };
};

template<typename T, size_t S>
class Pool
{
public:
    Pool() = default;
    virtual ~Pool() = default;
    int newObjectIndex()
    {
        AutoLock autoLock(mMutex);
        int index = -1;
        for(uint8_t pos = 0; pos < mPool.size(); ++pos)
        {
            if(mPool[pos].getState() == ePoolObjectState::FREE)
            {
                mPool[pos].allocateObject();
                index = pos;
                break;
            }
        }
        return index;
    }

    T* getObject(int index)
    {
        if(index < 0 && index >= mPool.size())
        {
            return nullptr;
        }
        return mPool[index].getObject();
    }

    void deleteObject(T* object)
    {
        AutoLock autoLock(mMutex);
        for(auto& obj: mPool)
        {
            if(obj.getObject() == object)
            {
                obj.freeObject();
            }
        }
    }

private:
    std::array<PoolObject<T>, S> mPool;
    Mutex mMutex;
};

#endif /* NEMEIO_TOOLS_POOL_HPP_ */
