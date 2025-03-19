#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "STM32HWTimer.hpp"
#include "mocks/MockSTM32HWTimerController.hpp"
#include "dummies/DummyRecursiveMutex.hpp"
#include "MockTimerListener.hpp"

using ::testing::NiceMock;
using ::testing::Return;

static constexpr uint32_t FREQ_1_MHZ = 1000000;

class FakeSTM32HWTimerController : public NiceMock<MockSTM32HWTimerController>
{
public:
    STM32HWTimerDefs::TimerId getTimerId() override { return STM32HWTimerDefs::TimerId::TIM_3; }
};

class STM32HWTimerTest : public ::testing::Test
{
protected:
    STM32HWTimerTest()
        : hwTimer(timController, mutex){};

    FakeSTM32HWTimerController timController;
    DummyRecursiveMutex mutex;
    STM32HWTimer hwTimer;
    MockTimerListener timerListener;
};

TEST_F(STM32HWTimerTest, SetDelayNominal)
{
    constexpr int32_t testDelayUs1 = 10;
    constexpr int32_t testDelayUs2 = 20;

    EXPECT_CALL(timController, init(testDelayUs1, FREQ_1_MHZ)).Times(1).WillOnce(Return(true));
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs1)), ITimer::RetCode::SUCCESS);

    EXPECT_CALL(timController, init(testDelayUs2, FREQ_1_MHZ)).Times(1).WillOnce(Return(true));
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs2)), ITimer::RetCode::SUCCESS);
};

TEST_F(STM32HWTimerTest, SetDelaySameValue)
{
    constexpr int32_t testDelayUs = 10;

    EXPECT_CALL(timController, init(testDelayUs, FREQ_1_MHZ)).Times(1).WillOnce(Return(true));
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs)), ITimer::RetCode::SUCCESS);

    EXPECT_CALL(timController, init(testDelayUs, FREQ_1_MHZ)).Times(0);
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs)), ITimer::RetCode::SUCCESS);
};

TEST_F(STM32HWTimerTest, SetDelayFailedInit)
{
    constexpr int32_t testDelayUs = 10;

    EXPECT_CALL(timController, init(testDelayUs, FREQ_1_MHZ)).Times(1).WillOnce(Return(false));
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs)), ITimer::RetCode::TIMER);

    // Check that next init is processed even with the same value
    EXPECT_CALL(timController, init(testDelayUs, FREQ_1_MHZ)).Times(1).WillOnce(Return(true));
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs)), ITimer::RetCode::SUCCESS);
};

TEST_F(STM32HWTimerTest, SetDelayInvalidValue)
{
    constexpr int32_t testDelayUs1 = 65537;
    constexpr int32_t testDelayUs2 = 0;

    EXPECT_CALL(timController, init(testDelayUs1, FREQ_1_MHZ)).Times(0);
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs1)), ITimer::RetCode::INVALID_PARAMETER);

    EXPECT_CALL(timController, init(testDelayUs1, FREQ_1_MHZ)).Times(0);
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs2)), ITimer::RetCode::INVALID_PARAMETER);
};

TEST_F(STM32HWTimerTest, StartStop)
{
    constexpr int32_t testDelayUs = 10;
    EXPECT_CALL(timController, init(testDelayUs, FREQ_1_MHZ)).Times(1).WillOnce(Return(true));
    ASSERT_EQ(hwTimer.setDelayUs(DelayUs(testDelayUs)), ITimer::RetCode::SUCCESS);

    EXPECT_CALL(timController, start).Times(1).WillOnce(Return(true));
    hwTimer.start();

    EXPECT_CALL(timController, stop).Times(1).WillOnce(Return(true));
    hwTimer.stop();
};

TEST_F(STM32HWTimerTest, UnitializedStartStop)
{
    EXPECT_CALL(timController, start).Times(0);
    hwTimer.start();

    EXPECT_CALL(timController, start).Times(0);
    hwTimer.stop();
};

TEST_F(STM32HWTimerTest, IRQ)
{
    EXPECT_CALL(timController, irqHandler).Times(1);
    STM32HWTimer::irqHandler(STM32HWTimerDefs::TimerId::TIM_3);
};

TEST_F(STM32HWTimerTest, periodElapsed)
{
    ON_CALL(timController, stop).WillByDefault(Return(true));

    hwTimer.registerSingleListener(timerListener);

    EXPECT_CALL(timerListener, timerCallback).Times(1);
    STM32HWTimer::periodElapsedCallback(STM32HWTimerDefs::TimerId::TIM_3);
};