#ifndef MOCKCRC32_HPP_
#define MOCKCRC32_HPP_

#include "gmock/gmock.h"
#include "ICRC32.hpp"

class MockCRC32 : public ICRC32
{
public:
    MOCK_METHOD(bool, compute, (const uint8_t* buffer, size_t len, uint32_t& crc32), (override));
    MOCK_METHOD(void, configure, (), (override));
    MOCK_METHOD(bool, lock, (DelayMs timeout), (override));
    MOCK_METHOD(bool, unlock, (), (override));
};

#endif /* MOCKCRC32_HPP_ */
