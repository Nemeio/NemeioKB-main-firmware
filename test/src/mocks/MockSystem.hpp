/*
 * MockKeepAliveManager.hpp
 *
 *  Created on: Dec 2, 2019
 *      Author: Labo
 */

#ifndef MOCKS_MOCKSYSTEM_HPP_
#define MOCKS_MOCKSYSTEM_HPP_

#include "gmock/gmock.h"
#include "ISystem.hpp"
#include "IUpdateListener.hpp"

class MockSystem : public ISystem
{
public:
    MOCK_METHOD(void, reset, (), (override));
    MOCK_METHOD(uint32_t, getTickCount, (), (override));
    MOCK_METHOD(uint32_t, delayToTicks, (const DelayMs& delay), (override));
    MOCK_METHOD(void, delay, (const DelayMs& delay), (override));
    MOCK_METHOD(void, disableInterrupt, (), (override));
    MOCK_METHOD(void, enableInterrupt, (), (override));
};

#endif /* MOCKS_MOCKSYSTEM_HPP_ */
