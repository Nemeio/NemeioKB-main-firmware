#ifndef MOCKS_MOCKSTREAMDATAREADER_HPP_
#define MOCKS_MOCKSTREAMDATAREADER_HPP_

#include "gmock/gmock.h"
#include "IStreamDataReader.hpp"

class MockStreamDataReader : public IStreamDataReader
{
public:
    MOCK_METHOD(const uint8_t*, read, (size_t size, size_t& retSize), (override));
    MOCK_METHOD(bool, getSize, (std::size_t & size), (override));

    MOCK_METHOD(bool, start, (), (override));
    MOCK_METHOD(bool, runRead, (), (override));
    MOCK_METHOD(void, notifyReadStart, (), (override));
    MOCK_METHOD(void, notifyReadFinish, (), (override));
    MOCK_METHOD(bool, waitReadFinish, (), (override));
};

#endif /* MOCKS_MOCKDATAREADER_HPP_ */
