/*
 * MockKeepAliveManager.hpp
 *
 *  Created on: Dec 2, 2019
 *      Author: Labo
 */

#ifndef MOCKS_MOCKKEEPALIVEMANAGER_HPP_
#define MOCKS_MOCKKEEPALIVEMANAGER_HPP_

#include "gmock/gmock.h"
#include "IKeepAliveManager.hpp"

class MockKeepAliveManager : public IKeepAliveManager
{
public:
    MOCK_METHOD(void, receivedKeepAlive, (), (override));
};

#endif /* MOCKS_MOCKKEEPALIVEMANAGER_HPP_ */
