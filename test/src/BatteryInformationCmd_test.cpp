#include "gtest/gtest.h"

#include "BatteryInformationCmd.hpp"
#include "MockAppCommTxListener.hpp"
#include "MockBattery.hpp"
#include "gtest_helpers.hpp"

using testing::_;
using testing::Args;
using ::testing::DoAll;
using testing::ElementsAreArray;
using ::testing::Return;
using ::testing::SetArgReferee;

class BatteryInformationCmdTest : public ::testing::Test
{
protected:
    BatteryInformationCmdTest()
        : batteryInformationCmd(txListener, battery){};

    static constexpr uint8_t LEVEL_PERCENT = 50;
    static constexpr uint16_t CAPACITY_MAH = 1300;
    static constexpr uint32_t TIME_TO_FULL_SEC = 0;
    static constexpr uint32_t TIME_TO_EMPTY_SEC = 3600;
    static constexpr uint16_t CYCLES_100TH = 70;
    static constexpr uint8_t AGE_PERCENT = 100;

    void nominalDataReceivedTest(uint8_t* pData, std::size_t dataLen)
    {
        BatteryInformation batteryInformation(LEVEL_PERCENT,
                                              CAPACITY_MAH,
                                              TIME_TO_FULL_SEC,
                                              TIME_TO_EMPTY_SEC,
                                              CYCLES_100TH,
                                              AGE_PERCENT);

        std::vector<uint8_t> resp;
        resp.push_back(AppCommDefs::AppCommErrCode::APPCOMMERR_SUCCESS);
        resp.push_back(LEVEL_PERCENT);
        pushBackU16BE(resp, CAPACITY_MAH);
        pushBackU32BE(resp, TIME_TO_FULL_SEC);
        pushBackU32BE(resp, TIME_TO_EMPTY_SEC);
        pushBackU16BE(resp, CYCLES_100TH);
        resp.push_back(AGE_PERCENT);

        EXPECT_CALL(battery, getBatteryInformation)
            .Times(1)
            .WillOnce(
                DoAll(SetArgReferee<0>(batteryInformation), Return(IBattery::RetCode::SUCCESS)));

        EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::BATTERYINFORMATION, _, _))
            .With(Args<1, 2>(ElementsAreArray(resp)))
            .Times(1);

        batteryInformationCmd.dataReceived(pData, dataLen);
    }

    void errorTest(IBattery::RetCode batteryRetCode, AppCommDefs::AppCommErrCode errCode)
    {
        uint8_t dummy = 0;
        uint8_t* pData = &dummy;
        size_t dataLen = 0;
        std::vector<uint8_t> resp;
        resp.push_back(errCode);

        EXPECT_CALL(battery, getBatteryInformation).Times(1).WillOnce(DoAll(Return(batteryRetCode)));

        EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::BATTERYINFORMATION, _, _))
            .With(Args<1, 2>(ElementsAreArray(resp)))
            .Times(1);

        batteryInformationCmd.dataReceived(pData, dataLen);
    }

    MockAppCommTxListener txListener;
    MockBattery battery;
    BatteryInformationCmd batteryInformationCmd;
};

TEST_F(BatteryInformationCmdTest, Nominal)
{
    uint8_t dummy = 0;
    uint8_t* pData = &dummy;
    size_t dataLen = 0;
    nominalDataReceivedTest(pData, dataLen);
}

TEST_F(BatteryInformationCmdTest, NominalNull)
{
    nominalDataReceivedTest(nullptr, 0);
}

TEST_F(BatteryInformationCmdTest, MalformedData)
{
    uint8_t dummy = 0;
    uint8_t* pData = &dummy;
    size_t dataLen = sizeof(dummy);

    std::vector<uint8_t> resp;
    resp.push_back(AppCommDefs::AppCommErrCode::APPCOMMERR_MALFORMED_DATA);

    EXPECT_CALL(battery, getBatteryInformation).Times(0);

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::BATTERYINFORMATION, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    batteryInformationCmd.dataReceived(pData, dataLen);
}

TEST_F(BatteryInformationCmdTest, BatteryInformationNotReadyError)
{
    uint8_t dummy = 0;
    uint8_t* pData = &dummy;
    size_t dataLen = sizeof(dummy);
    errorTest(IBattery::RetCode::INFORMATION_NOT_READY,
              AppCommDefs::AppCommErrCode::APPCOMMERR_BATTERY_NOT_READY_ERROR);
}

TEST_F(BatteryInformationCmdTest, BatteryInformationFuelGaugeError)
{
    uint8_t dummy = 0;
    uint8_t* pData = &dummy;
    size_t dataLen = sizeof(dummy);
    errorTest(IBattery::RetCode::FUELGAUGE_ERROR,
              AppCommDefs::AppCommErrCode::APPCOMMERR_BATTERY_FUEL_GAUGE_ERROR);
}
