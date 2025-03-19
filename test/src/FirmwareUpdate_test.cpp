#include "gtest/gtest.h"

#include "STM32FirmwareUpdate.hpp"
#include "MockFlashInterface.hpp"
#include "MockSFU.hpp"
#include "MockUpdateListener.hpp"
#include "se_def.h"

using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrictMock;

class FirmwareUpdateTest : public ::testing::Test
{
protected:
    FirmwareUpdateTest()
        : firmwareUpdate(flashInterface, sfu)
        , dummyHeader()
        , dummyAreaInfo(){};

    void dummyInit()
    {
        dummyAreaInfo.ImageAddr = reinterpret_cast<uint8_t*>(&dummyHeader);

        EXPECT_CALL(sfu, getDownloadAreaInfo)
            .Times(1)
            .WillOnce(DoAll(SetArgPointee<0>(dummyAreaInfo), Return(true)));
        EXPECT_CALL(flashInterface, eraseSize).WillOnce(Return(true));

        std::size_t dummyLen = 0;
        EXPECT_EQ(firmwareUpdate.initUpdate(dummyLen), FirmwareUpdateDefs::ErrCode::SUCCESS);
    }

    MockFlashInterface flashInterface;
    MockSFU sfu;
    STM32FirmwareUpdate firmwareUpdate;

    SE_FwRawHeaderTypeDef dummyHeader;
    SFU_FwImageFlashTypeDef dummyAreaInfo;
};

TEST_F(FirmwareUpdateTest, RunUpdateOk)
{
    dummyInit();

    EXPECT_EQ(firmwareUpdate.runUpdate(), FirmwareUpdateDefs::ErrCode::SUCCESS);
}