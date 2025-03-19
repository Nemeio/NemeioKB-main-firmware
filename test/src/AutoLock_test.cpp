#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gtest_helpers.hpp"

#include "AutoLock.h"
#include "MockMutex.hpp"

using ::testing::InSequence;
using ::testing::Return;

class IAction
{
    virtual void doAction() = 0;
};

class MockAction : public IAction
{
public:
    MOCK_METHOD(void, doAction, (), (override));
};

class AutoLockTest : public ::testing::Test
{
protected:
    AutoLockTest() {}

    void test()
    {
        AutoLock autoLock(mutex);
        action.doAction();
    }

    MockMutex mutex;
    MockAction action;
};

TEST_F(AutoLockTest, Nominal)
{
    {
        InSequence seq;
        EXPECT_CALL(mutex, doLock).Times(1).WillOnce(Return(true));
        EXPECT_CALL(action, doAction).Times(1);
        EXPECT_CALL(mutex, unlock).Times(1).WillOnce(Return(true));
    }
    test();
};

TEST_F(AutoLockTest, LockFailure)
{
    EXPECT_CALL(mutex, doLock).WillRepeatedly(Return(false));

    EXPECT_ASSERTION_FAILED(test());
};

TEST_F(AutoLockTest, UnlockFailure)
{
    EXPECT_CALL(mutex, doLock).WillRepeatedly(Return(true));
    EXPECT_CALL(action, doAction).WillRepeatedly(Return());
    EXPECT_CALL(mutex, unlock).WillRepeatedly(Return(false));

    EXPECT_ASSERTION_FAILED(test());
};