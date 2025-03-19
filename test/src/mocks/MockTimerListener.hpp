/*
 * MockKeepAliveManager.hpp
 *
 *  Created on: Dec 2, 2019
 *      Author: Labo
 */

#ifndef MOCKS_MOCKTIMERLISTENER_HPP_
#define MOCKS_MOCKTIMERLISTENER_HPP_

#include "gmock/gmock.h"
#include "ITimerListener.hpp"

class MockTimerListener : public ITimerListener
{
public:
    MOCK_METHOD(void, timerCallback, (), (override));
};

#endif /* MOCKS_MOCKTIMERLISTENER_HPP_ */
