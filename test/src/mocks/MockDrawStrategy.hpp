#ifndef MOCKS_MOCKDRAWSTRATEGY_HPP_
#define MOCKS_MOCKDRAWSTRATEGY_HPP_

#include "gmock/gmock.h"
#include "IDrawStrategy.hpp"

class MockDrawStrategy : public IDrawStrategy
{
public:
    MOCK_METHOD(ScreenDefs::ScreenBpp, getBpp, (), (override));
    MOCK_METHOD(uint8_t, getNbImages, (), (override));
    MOCK_METHOD(bool,
                convertImageIdxToFbNum,
                (std::size_t imageIdx, LayoutModifiersDefs::FrameBufferIndex& fbNum),
                (override));

#endif /* MOCKS_MOCKSIGNAL_HPP_ */