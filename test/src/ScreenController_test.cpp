#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "IScreenController.hpp"
#include "MockScreen.hpp"
#include <math.h>
#include <tuple>
#include "MockStreamDataReader.hpp"
#include "WriteBuffer.hpp"
#include "ImageTestValues.hpp"

using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::StrictMock;

class FakeDrawStrategy : public IDrawStrategy
{
public:
    FakeDrawStrategy(uint8_t nbImages, ScreenDefs::PixelFormat bpp)
        : mNbImages(nbImages)
        , mBpp(bpp){};

    uint8_t getNbImages() const { return mNbImages; }

    ScreenDefs::PixelFormat getBpp() const { return mBpp; }

    bool convertImageIdxToFbNum(std::size_t imageIdx, uint8_t& fbNum) const
    {
        fbNum = imageIdx;
        return true;
    }

private:
    const uint8_t mNbImages;
    const ScreenDefs::PixelFormat mBpp;
};

class MockStreamObject : public StreamObject<MockStreamObject>
{
public:
    template<std::size_t READ_BUFF_SIZE,
             std::size_t STREAM_BUFF_SIZE,
             int32_t STREAM_TIMEOUT,
             typename InitCallback,
             typename ReadCallback>
    bool doRead(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                std::size_t streamTriggerLevel,
                std::array<uint8_t, READ_BUFF_SIZE>& sourceReadChunkBuffer,
                InitCallback initCb,
                ReadCallback readCb)
    {
        return this->commonRead(reader, initCb, readCb);
    };

    MockStreamDataReader reader;
};

class ScreenControllerTest : public ::testing::Test
{
protected:
    ScreenControllerTest()
        : screenController(screen){};

    ScreenController screenController;
    MockScreen screen;
};

class ScreenControllerMultipleRefreshTestP : public ScreenControllerTest,
                                             public ::testing::WithParamInterface<uint8_t>
{
public:
    static constexpr uint8_t NB_MIN_REFRESH = 3;

    void refreshCycle(uint8_t cleanRefreshPeriod,
                      uint8_t nbRefresh,
                      ScreenDefs::PixelFormat pixelFormat,
                      uint8_t fb)
    {
        for(uint8_t i = 0; i < nbRefresh; i++)
        {
            bool bCleanRefreshExpect = ((cleanRefreshPeriod == 0) || (i % cleanRefreshPeriod == 0));

            EXPECT_CALL(screen, refresh(bCleanRefreshExpect, pixelFormat, fb))
                .Times(1)
                .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
            EXPECT_EQ(screenController.refresh(pixelFormat, fb), ScreenDefs::ScreenErrCode::SUCCESS);
        }
    }
};

class ScreenControllerLowPowerDelayTestP
    : public ScreenControllerTest,
      public ::testing::WithParamInterface<std::tuple<uint16_t, uint32_t>>
{
};

TEST_F(ScreenControllerTest, InitNominal)
{
    EXPECT_CALL(screen, init).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_EQ(screenController.init(), ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, RefreshNominal)
{
    EXPECT_CALL(screen, refresh(true, ScreenDefs::PixelFormat::_1BPP, 0))
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_EQ(screenController.refresh(ScreenDefs::PixelFormat::_1BPP, 0),
              ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, RefreshWindowNominal)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);

    EXPECT_CALL(screen, refreshWindow).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_EQ(screenController.refreshWindow(window, true, ScreenDefs::PixelFormat::_1BPP, 0),
              ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, ClearCompleteScreenNominal)
{
    EXPECT_CALL(screen, clearCompleteScreen)
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_EQ(screenController.clearCompleteScreen(), ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, DrawNominal)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);

    EXPECT_CALL(screen, draw).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_EQ(screenController.draw(window, nullptr, 0, ScreenDefs::PixelFormat::_1BPP, 0),
              ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, MultipleDrawNominal)
{
    ScreenController::DrawArgs drawArgsList[3] = {};
    EXPECT_CALL(screen, clearCompleteScreen)
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_CALL(screen, draw).Times(3).WillRepeatedly(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_CALL(screen, refresh).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_EQ(screenController.multipleDraw(std::begin(drawArgsList),
                                            std::end(drawArgsList),
                                            ScreenDefs::PixelFormat::_1BPP,
                                            0),
              ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_P(ScreenControllerMultipleRefreshTestP, MultipleRefresh)
{
    uint8_t cleanRefreshPeriod = GetParam();
    const uint8_t nbRefreshTest = std::max((const uint8_t)(cleanRefreshPeriod * 2),
                                           ScreenControllerMultipleRefreshTestP::NB_MIN_REFRESH);

    screenController.setCleanRefreshPeriod(cleanRefreshPeriod);

    refreshCycle(cleanRefreshPeriod, nbRefreshTest, ScreenDefs::PixelFormat::_1BPP, 0);
};

INSTANTIATE_TEST_SUITE_P(ScreenControllerMultipleRefreshTestSuite,
                         ScreenControllerMultipleRefreshTestP,
                         testing::Values(0, 1, 2, 5));

TEST_P(ScreenControllerMultipleRefreshTestP, MultipleRefreshResetPeriod)
{
    EXPECT_CALL(screen, refresh(true, ScreenDefs::PixelFormat::_1BPP, 0))
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    EXPECT_EQ(screenController.refresh(ScreenDefs::PixelFormat::_1BPP, 0),
              ScreenDefs::ScreenErrCode::SUCCESS);

    uint8_t cleanRefreshPeriod = GetParam();

    screenController.setCleanRefreshPeriod(cleanRefreshPeriod);

    refreshCycle(cleanRefreshPeriod, cleanRefreshPeriod, ScreenDefs::PixelFormat::_1BPP, 0);
};

INSTANTIATE_TEST_SUITE_P(ScreenControllerMultipleRefreshResetPeriodTestSuite,
                         ScreenControllerMultipleRefreshTestP,
                         testing::Values(5));

TEST_F(ScreenControllerTest, LowPower)
{
    EXPECT_CALL(screen, wakeUp).Times(0);
    screenController.wakeUp();

    EXPECT_CALL(screen, goToLowPowerMode)
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    screenController.goToLowPowerMode();

    EXPECT_CALL(screen, goToLowPowerMode).Times(0);
    screenController.goToLowPowerMode();

    EXPECT_CALL(screen, wakeUp).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
    screenController.wakeUp();

    EXPECT_CALL(screen, wakeUp).Times(0);
    screenController.wakeUp();
};

TEST_F(ScreenControllerTest, LowPowerDelayInfinite)
{
    screenController.setDisplayLowPowerDelaySec(0);

    DelayMs delay = screenController.getDisplayLowPowerDelay();

    EXPECT_TRUE(delay.isInfiniteDelay());
};

TEST_P(ScreenControllerLowPowerDelayTestP, LowPowerDelay)
{
    uint16_t delaySec = std::get<0>(GetParam());
    uint32_t resultDelayMs = std::get<1>(GetParam());

    ASSERT_TRUE(delaySec != 0);

    screenController.setDisplayLowPowerDelaySec(delaySec);

    DelayMs delay = screenController.getDisplayLowPowerDelay();

    EXPECT_FALSE(delay.isInfiniteDelay());
    EXPECT_EQ(delay.getMs(), resultDelayMs);
};

INSTANTIATE_TEST_SUITE_P(ScreenControllerLowPowerDelayTestSuite,
                         ScreenControllerLowPowerDelayTestP,
                         testing::Values(std::make_tuple(1, 1000), std::make_tuple(5, 5000)));

TEST_F(ScreenControllerTest, DisplayStreamNominal)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_1BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_1BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(1).WillOnce(Return(true));
        EXPECT_CALL(screen, draw).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(screen, refresh).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(1).WillOnce(Return(true));
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, DisplayStreamWrongSize)
{
    ScreenDefs::Window window(0, 0, 10, 20);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_1BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_1BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(0);
        EXPECT_CALL(screen, draw).Times(0);
        EXPECT_CALL(screen, refresh).Times(0);
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(0);
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::INVALID_PARAMETER);
};

TEST_F(ScreenControllerTest, DisplayStreamGetSizeFail)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_1BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_1BPP), Return(false)));
        EXPECT_CALL(streamObject.reader, start).Times(0);
        EXPECT_CALL(screen, draw).Times(0);
        EXPECT_CALL(screen, refresh).Times(0);
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(0);
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::READ_DATA_FAILED);
};

TEST_F(ScreenControllerTest, DisplayStreamStartFail)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_1BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_1BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(1).WillOnce(Return(false));
        EXPECT_CALL(screen, draw).Times(0);
        EXPECT_CALL(screen, refresh).Times(0);
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(0);
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::READ_DATA_FAILED);
};

TEST_F(ScreenControllerTest, DisplayStreamDrawFail)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_1BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_1BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(1).WillOnce(Return(true));
        EXPECT_CALL(screen, draw)
            .Times(1)
            .WillOnce(Return(ScreenDefs::ScreenErrCode::NOT_RESPONDING));
        EXPECT_CALL(screen, refresh).Times(0);
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(1).WillOnce(Return(true));
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::NOT_RESPONDING);
};

TEST_F(ScreenControllerTest, DisplayStreamDrawFailWaitReadFinishFail)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_1BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_1BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(1).WillOnce(Return(true));
        EXPECT_CALL(screen, draw)
            .Times(1)
            .WillOnce(Return(ScreenDefs::ScreenErrCode::NOT_RESPONDING));
        EXPECT_CALL(screen, refresh).Times(0);
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(1).WillOnce(Return(false));
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::NOT_RESPONDING);
};

TEST_F(ScreenControllerTest, DisplayStreamRefreshFail)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_1BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_1BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(1).WillOnce(Return(true));
        EXPECT_CALL(screen, draw).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(screen, refresh)
            .Times(1)
            .WillOnce(Return(ScreenDefs::ScreenErrCode::NOT_RESPONDING));
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(1).WillOnce(Return(true));
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::NOT_RESPONDING);
};

TEST_F(ScreenControllerTest, DisplayStreamWaitReadFinishFail)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_1BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_1BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(1).WillOnce(Return(true));
        EXPECT_CALL(screen, draw).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(screen, refresh).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(1).WillOnce(Return(false));
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::READ_DATA_FAILED);
};

TEST_F(ScreenControllerTest, DisplayStreamNominal2BPP)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    FakeDrawStrategy drawStrategy(1, ScreenDefs::PixelFormat::_2BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(Image::SIZE_2BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(1).WillOnce(Return(true));
        EXPECT_CALL(screen, draw).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(screen, refresh).Times(1).WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(1).WillOnce(Return(true));
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, DisplayStreamNominalMultipleImages)
{
    ScreenDefs::Window window(0, 0, Image::WIDTH, Image::HEIGHT);
    MockStreamObject streamObject;
    constexpr uint8_t NB_IMAGES = 6;
    FakeDrawStrategy drawStrategy(NB_IMAGES, ScreenDefs::PixelFormat::_2BPP);

    {
        InSequence seq;
        EXPECT_CALL(streamObject.reader, getSize)
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<0>(NB_IMAGES * Image::SIZE_2BPP), Return(true)));
        EXPECT_CALL(streamObject.reader, start).Times(1).WillOnce(Return(true));
        EXPECT_CALL(screen, draw).Times(1).WillRepeatedly(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(screen, refresh(true, ScreenDefs::PixelFormat::_2BPP, 0))
            .Times(1)
            .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(screen, draw)
            .Times(NB_IMAGES - 1)
            .WillRepeatedly(Return(ScreenDefs::ScreenErrCode::SUCCESS));
        EXPECT_CALL(streamObject.reader, waitReadFinish).Times(1).WillOnce(Return(true));
    }

    EXPECT_EQ(screenController.drawFromStream(streamObject, window, drawStrategy),
              ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, UpgradeStartNominal)
{
    EXPECT_CALL(screen, onUpdateStarted())
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));

    EXPECT_EQ(screenController.onUpdateStarted(), ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, UpgradeStartFailed)
{
    EXPECT_CALL(screen, onUpdateStarted())
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::UPGRADE_FAILED));

    EXPECT_EQ(screenController.onUpdateStarted(), ScreenDefs::ScreenErrCode::UPGRADE_FAILED);
};

TEST_F(ScreenControllerTest, UpgradeEndNominal)
{
    EXPECT_CALL(screen, onUpdateEnded())
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::SUCCESS));

    EXPECT_EQ(screenController.onUpdateEnded(), ScreenDefs::ScreenErrCode::SUCCESS);
};

TEST_F(ScreenControllerTest, UpgradeEndFailed)
{
    EXPECT_CALL(screen, onUpdateEnded())
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::UPGRADE_FAILED));

    EXPECT_EQ(screenController.onUpdateEnded(), ScreenDefs::ScreenErrCode::UPGRADE_FAILED);
};
