/*
 * MockAppCommTxListener.hpp
 *
 *  Created on: Dec 2, 2019
 *      Author: Labo
 */

#ifndef MOCKS_MOCKAPPCOMMTXLISTENER_HPP_
#define MOCKS_MOCKAPPCOMMTXLISTENER_HPP_

#include "gmock/gmock.h"
#include "IAppCommTxListener.hpp"

class MockAppCommTxListener : public IAppCommTxListener
{
public:
    MOCK_METHOD(void,
                sendCmd,
                (AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len),
                (override));
    MOCK_METHOD(void,
                sendResponse,
                (AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len),
                (override));
};

#endif /* MOCKS_MOCKAPPCOMMTXLISTENER_HPP_ */
