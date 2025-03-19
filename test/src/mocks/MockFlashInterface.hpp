#ifndef MOCKS_MOCKFLASHINTERFACE_HPP_
#define MOCKS_MOCKFLASHINTERFACE_HPP_

#include "gmock/gmock.h"
#include "IFlashInterface.hpp"

class MockFlashInterface : public IFlashInterface
{
public:
    MOCK_METHOD(bool,
                write,
                (void* pDestination, const void* pSource, uint32_t uLength),
                (override));
    MOCK_METHOD(bool, eraseSize, (void* pStart, uint32_t uLength), (override));

    MOCK_METHOD(void*, getEnd, (), (const, override));
    MOCK_METHOD(size_t, getPageSize, (), (const, override));
    MOCK_METHOD(bool, isFlashZone, (void* pStart, uint32_t uLength), (const, override));
    MOCK_METHOD(bool, isFlashErasableZone, (void* pStart, uint32_t uLength), (const, override));
};

#endif /* MOCKS_MOCKFLASHINTERFACE_HPP_ */