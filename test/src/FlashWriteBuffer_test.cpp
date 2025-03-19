#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "FlashWriteBuffer.hpp"

using ::testing::NiceMock;
using ::testing::StrictMock;

class FlashWriteBufferTest : public ::testing::Test
{
public:
    size_t _bufferFull(const size_t* data, size_t size)
    {
        for(size_t index = 0; index < size; ++index)
        {
            EXPECT_EQ(data[index], mContent[mCurrentIndex++]);
        }

        mDataReceived += size;
        return size;
    }

protected:
    FlashWriteBufferTest()
        : mFlashWriteBuffer(std::bind(&FlashWriteBufferTest::_bufferFull,
                                      this,
                                      std::placeholders::_1,
                                      std::placeholders::_2))
    {
        for(size_t index = 0; index < mContent.size(); ++index)
        {
            mContent[index] = index;
        }
    };

    std::array<size_t, 1024> mContent;
    FlashWriteBuffer<size_t, 256> mFlashWriteBuffer;
    size_t mCurrentIndex = 0;
    size_t mDataReceived = 0;
};

TEST_F(FlashWriteBufferTest, MultiplePart)
{
    mFlashWriteBuffer.pushBack(mContent.begin(), mContent.end());
    EXPECT_EQ(mContent.size(), mDataReceived);
};

TEST_F(FlashWriteBufferTest, FlushPartial)
{
    size_t partialSize = mContent.size() / 2;
    mFlashWriteBuffer.pushBack(mContent.begin(), mContent.begin() + partialSize);
    mFlashWriteBuffer.partialFlush();
    EXPECT_EQ(mDataReceived, partialSize);
};
