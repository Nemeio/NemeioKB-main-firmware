/*
 * MockUpdateListener.hpp
 *
 *  Created on: Fev 12, 2021
 *      Author: thomas
 */

#ifndef MOCKS_MOCKUPDATELISTENER_HPP_
#define MOCKS_MOCKUPDATELISTENER_HPP_

#include "gmock/gmock.h"
#include "ISystem.hpp"
#include "IUpdateListener.hpp"

class MockUpdateListener : public IUpdateListener
{
public:
    MOCK_METHOD(void, onUpdateEvent, (UpdateStatus status), (override));
};

#endif /* MOCKS_MOCKUPDATELISTENER_HPP_ */
