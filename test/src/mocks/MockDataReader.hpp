#ifndef MOCKS_MOCKDATAREADER_HPP_
#define MOCKS_MOCKDATAREADER_HPP_

#include "gmock/gmock.h"
#include "IDataReader.hpp"

class MockDataReader : public IDataReader
{
public:
    MOCK_METHOD(const uint8_t*, read, (size_t size, size_t& retSize), (override));
    MOCK_METHOD(bool, getSize, (std::size_t & size), (override));
};

#endif /* MOCKS_MOCKDATAREADER_HPP_ */
