#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "DoubleClickKeyHandler.hpp"
#include "Mutex.hpp"
#include "MockTimer.hpp"
#include "MockWorkQueue.hpp"
#include "MockDoubleClickKeyListener.hpp"

using ::testing::DoAll;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::StrictMock;

class DoubleClickKeyHandlerTest : public ::testing::Test
{
protected:
    DoubleClickKeyHandlerTest()
        : handler(timer, mutex, workQueue)
        , defaultLayoutConfiguration(uuid::UuidView{defaultUuid},
                                     uuid::UuidView{defaultUuid},
                                     etl::nullopt,
                                     configuration::OperatingSystem::WINDOWS,
                                     defaultKeySequence,
                                     defaultKeySequence)
    {
        handler.registerObserver(listener);
    }

    ButtonID button{5, 6};
    StrictMock<MockDoubleClickKeyListener> listener;
    NiceMock<MockTimer> timer;
    Mutex mutex;
    NiceMock<MockWorkQueue> workQueue;
    DoubleClickKeyHandler handler;
    uuid::Uuid defaultUuid{etl::array<std::byte, uuid::SIZE>{std::byte(0x00)}};
    etl::vector<configuration::KeySequence, 1> defaultKeySequence;
    configuration::LayoutConfiguration defaultLayoutConfiguration;
};

TEST_F(DoubleClickKeyHandlerTest, SimpleClick)
{
    Work capturedWorkTimeout;
    Work capturedWorkSimpleClick;

    EXPECT_CALL(timer, start).Times(1);
    handler.pressedKeyWithConfig(button, defaultLayoutConfiguration);
    Mock::VerifyAndClearExpectations(&timer);

    handler.unpressedKeyWithConfig(button, defaultLayoutConfiguration);

    EXPECT_CALL(workQueue, submit)
        .Times(1)
        .WillOnce(DoAll(SaveArg<0>(&capturedWorkTimeout), Return(true)));
    handler.timerCallback();
    Mock::VerifyAndClearExpectations(&workQueue);

    EXPECT_CALL(workQueue, submit)
        .Times(1)
        .WillOnce(DoAll(SaveArg<0>(&capturedWorkSimpleClick), Return(true)));

    capturedWorkTimeout.run(etl::array_view<uint8_t>());
    Mock::VerifyAndClearExpectations(&workQueue);

    EXPECT_CALL(listener, simpleClicked(button)).Times(1);
    capturedWorkSimpleClick.run(etl::array_view<uint8_t>());
};

TEST_F(DoubleClickKeyHandlerTest, DoubleClick)
{
    Work capturedWorkDoubleClick;

    EXPECT_CALL(timer, start).Times(1);
    handler.pressedKeyWithConfig(button, defaultLayoutConfiguration);
    Mock::VerifyAndClearExpectations(&timer);

    handler.unpressedKeyWithConfig(button, defaultLayoutConfiguration);


    EXPECT_CALL(workQueue, submit)
        .Times(1)
        .WillOnce(DoAll(SaveArg<0>(&capturedWorkDoubleClick), Return(true)));
    EXPECT_CALL(timer, stop).Times(1);
    handler.pressedKeyWithConfig(button, defaultLayoutConfiguration);

    Mock::VerifyAndClearExpectations(&workQueue);
    Mock::VerifyAndClearExpectations(&timer);

    EXPECT_CALL(listener, doubleClicked(button)).Times(1);
    capturedWorkDoubleClick.run(etl::array_view<uint8_t>());

    Mock::VerifyAndClearExpectations(&listener);

    // Nothing should happen
    handler.unpressedKeyWithConfig(button, defaultLayoutConfiguration);
};