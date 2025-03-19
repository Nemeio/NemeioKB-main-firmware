#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "HWTimerWaitHelper.hpp"
#include "mocks/MockHWTimer.hpp"
#include "mocks/MockSignal.hpp"

using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;

static constexpr uint32_t FREQ_1_MHZ = 1000000;

class HWTimerWaitHelperTest : public ::testing::Test
{
protected:
    HWTimerWaitHelperTest()
        : waitHelper(hwTimer, signal){};

    NiceMock<MockHWTimer> hwTimer;
    NiceMock<MockSignal> signal;
    HWTimerWaitHelper waitHelper;
};

TEST_F(HWTimerWaitHelperTest, Nominal)
{
    DelayUs delayTimer(5);
    DelayMs delayWait(1);

    {
        InSequence s;
        EXPECT_CALL(hwTimer, setDelayUs(delayTimer))
            .Times(1)
            .WillOnce(Return(ITimer::RetCode::SUCCESS));

        EXPECT_CALL(hwTimer, stop).Times(1);
        EXPECT_CALL(hwTimer, start).Times(1);

        EXPECT_CALL(signal, wait(delayWait)).WillOnce(Return(true));
    }
    EXPECT_EQ(waitHelper.wait(delayTimer, delayWait), HWTimerWaitHelper::RetCode::SUCCESS);
};

TEST_F(HWTimerWaitHelperTest, InvalidMaxOsDelay)
{
    EXPECT_CALL(hwTimer, start).Times(0);
    EXPECT_EQ(waitHelper.wait(DelayUs(1000), DelayMs(1)),
              HWTimerWaitHelper::RetCode::INVALID_PARAMETER);
};

TEST_F(HWTimerWaitHelperTest, InvalidDelay)
{
    DelayUs delayTimer(5);

    EXPECT_CALL(hwTimer, setDelayUs(delayTimer))
        .Times(1)
        .WillOnce(Return(ITimer::RetCode::INVALID_PARAMETER));
    EXPECT_CALL(hwTimer, start).Times(0);
    EXPECT_EQ(waitHelper.wait(delayTimer), HWTimerWaitHelper::RetCode::INVALID_PARAMETER);
};

TEST_F(HWTimerWaitHelperTest, SetDelayError)
{
    DelayUs delayTimer(5);

    EXPECT_CALL(hwTimer, setDelayUs(delayTimer)).Times(1).WillOnce(Return(ITimer::RetCode::TIMER));

    EXPECT_CALL(hwTimer, start).Times(0);

    EXPECT_EQ(waitHelper.wait(delayTimer), HWTimerWaitHelper::RetCode::TIMER);
};

TEST_F(HWTimerWaitHelperTest, WaitError)
{
    DelayUs delayTimer(5);
    DelayMs delayWait(1);

    EXPECT_CALL(hwTimer, setDelayUs(delayTimer)).Times(1).WillOnce(Return(ITimer::RetCode::SUCCESS));

    EXPECT_CALL(hwTimer, start).Times(1);

    EXPECT_CALL(signal, wait(delayWait)).Times(1).WillOnce(Return(false));

    EXPECT_EQ(waitHelper.wait(delayTimer, delayWait), HWTimerWaitHelper::RetCode::TIMEOUT);
};

TEST_F(HWTimerWaitHelperTest, TimerCallback)
{
    EXPECT_CALL(signal, setFromISR).Times(1);
    waitHelper.timerCallback();
};