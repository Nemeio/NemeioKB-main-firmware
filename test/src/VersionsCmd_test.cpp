#include "gtest/gtest.h"
#include "gtest_helpers.hpp"

#include "VersionsCmd.hpp"
#include "MockAppCommTxListener.hpp"
#include "MockBleChip.hpp"
#include "MockScreenVersionsReader.hpp"
#include <memory>

using ::testing::_;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAreArray;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::SetArrayArgument;

class VersionsCmdTest : public ::testing::Test
{
protected:
    MockAppCommTxListener txListener;
    MockBleChip bleChip;
    MockScreenVersionsReader screenVersionsReader;
    VersionsCmd versionsCmd;

    VersionsCmdTest()
        : versionsCmd(txListener, bleChip, screenVersionsReader)
    {
    }

    static constexpr char cpuVersion[VERSION_MAX_SZ] = "43.21";
    static constexpr char bleVersionStr[VERSION_MAX_SZ] = "12.30";
    static constexpr BLEDefs::FirmwareVersion bleVersion{12, 30};
    static constexpr char displayChipFirmwareVersion[VERSION_MAX_SZ] = "LDLC_v.0.3T5";
    static constexpr char displayChipLUTVersion[VERSION_MAX_SZ] = "6M14T_Holitech";
};

TEST_F(VersionsCmdTest, Nominal)
{
    uint8_t* pData = nullptr;
    size_t dataLen = 0;

    std::vector<uint8_t> resp;
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(), cpuVersion, cpuVersion + sizeof(cpuVersion));
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(), bleVersionStr, bleVersionStr + sizeof(bleVersionStr));
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(),
                displayChipFirmwareVersion,
                displayChipFirmwareVersion + sizeof(displayChipFirmwareVersion));
    resp.insert(resp.end(),
                displayChipLUTVersion,
                displayChipLUTVersion + sizeof(displayChipLUTVersion));

    auto bleGetFwVersionResponse = [this](auto request)
    {
        versionsCmd.notification(BleFwVersionEvent{bleVersion});
        return BLEDefs::ErrCode::SUCCESS;
    };

    EXPECT_CALL(bleChip, sendRequest(IsBleCmdVariant(BLESPICommands::CommandId::GET_FW_VERSION)))
        .Times(1)
        .WillOnce(Invoke(bleGetFwVersionResponse));
    EXPECT_CALL(txListener, sendResponse).Times(0);

    EXPECT_CALL(screenVersionsReader, getVersions)
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<0>(displayChipFirmwareVersion,
                                            displayChipFirmwareVersion
                                                + sizeof(displayChipFirmwareVersion)),
                        SetArrayArgument<2>(displayChipLUTVersion,
                                            displayChipLUTVersion + sizeof(displayChipLUTVersion)),
                        Return(ScreenDefs::ScreenErrCode::SUCCESS)));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::VERSIONS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    versionsCmd.dataReceived(pData, dataLen);
}

TEST_F(VersionsCmdTest, BleTimeout)
{
    uint8_t* pData = nullptr;
    size_t dataLen = 0;

    static constexpr char noBleVersion[VERSION_MAX_SZ] = "";
    std::vector<uint8_t> resp;
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(), cpuVersion, cpuVersion + sizeof(cpuVersion));
    resp.push_back(AppCommDefs::APPCOMMERR_BLE_CHIP_COMM_ERROR);
    resp.insert(resp.end(), noBleVersion, noBleVersion + sizeof(noBleVersion));
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(),
                displayChipFirmwareVersion,
                displayChipFirmwareVersion + sizeof(displayChipFirmwareVersion));
    resp.insert(resp.end(),
                displayChipLUTVersion,
                displayChipLUTVersion + sizeof(displayChipLUTVersion));

    auto bleGetFwVersionTimeout = [this](auto request)
    {
        versionsCmd.notification(BleTxRequestTimeout{BLESPICommands::CommandId::GET_FW_VERSION});
        return BLEDefs::ErrCode::SUCCESS;
    };

    EXPECT_CALL(bleChip, sendRequest(IsBleCmdVariant(BLESPICommands::CommandId::GET_FW_VERSION)))
        .Times(1)
        .WillOnce(Invoke(bleGetFwVersionTimeout));

    EXPECT_CALL(screenVersionsReader, getVersions)
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<0>(displayChipFirmwareVersion,
                                            displayChipFirmwareVersion
                                                + sizeof(displayChipFirmwareVersion)),
                        SetArrayArgument<2>(displayChipLUTVersion,
                                            displayChipLUTVersion + sizeof(displayChipLUTVersion)),
                        Return(ScreenDefs::ScreenErrCode::SUCCESS)));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::VERSIONS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    versionsCmd.dataReceived(pData, dataLen);
}

TEST_F(VersionsCmdTest, BleNotReady)
{
    uint8_t* pData = nullptr;
    size_t dataLen = 0;

    static constexpr char noBleVersion[VERSION_MAX_SZ] = "";
    std::vector<uint8_t> resp;
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(), cpuVersion, cpuVersion + sizeof(cpuVersion));
    resp.push_back(AppCommDefs::APPCOMMERR_BLE_CHIP_COMM_ERROR);
    resp.insert(resp.end(), noBleVersion, noBleVersion + sizeof(noBleVersion));
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(),
                displayChipFirmwareVersion,
                displayChipFirmwareVersion + sizeof(displayChipFirmwareVersion));
    resp.insert(resp.end(),
                displayChipLUTVersion,
                displayChipLUTVersion + sizeof(displayChipLUTVersion));

    EXPECT_CALL(bleChip, sendRequest(IsBleCmdVariant(BLESPICommands::CommandId::GET_FW_VERSION)))
        .Times(1)
        .WillOnce(Return(BLEDefs::ErrCode::DEVICE_NOT_READY));

    EXPECT_CALL(screenVersionsReader, getVersions)
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<0>(displayChipFirmwareVersion,
                                            displayChipFirmwareVersion
                                                + sizeof(displayChipFirmwareVersion)),
                        SetArrayArgument<2>(displayChipLUTVersion,
                                            displayChipLUTVersion + sizeof(displayChipLUTVersion)),
                        Return(ScreenDefs::ScreenErrCode::SUCCESS)));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::VERSIONS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    versionsCmd.dataReceived(pData, dataLen);
}

TEST_F(VersionsCmdTest, DisplayChipVersionsError)
{
    uint8_t dummy = 0;
    uint8_t* pData = &dummy;
    size_t dataLen = 0;

    static constexpr char noDisplayChipVersion[VERSION_MAX_SZ] = "";

    std::vector<uint8_t> resp;
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(), cpuVersion, cpuVersion + sizeof(cpuVersion));
    resp.push_back(AppCommDefs::APPCOMMERR_SUCCESS);
    resp.insert(resp.end(), bleVersionStr, bleVersionStr + sizeof(bleVersionStr));
    resp.push_back(AppCommDefs::APPCOMMERR_DISPLAY_NOT_READY);
    resp.insert(resp.end(),
                noDisplayChipVersion,
                noDisplayChipVersion + sizeof(noDisplayChipVersion));
    resp.insert(resp.end(),
                noDisplayChipVersion,
                noDisplayChipVersion + sizeof(noDisplayChipVersion));

    auto bleGetFwVersionResponse = [this](auto request)
    {
        versionsCmd.notification(BleFwVersionEvent{bleVersion});
        return BLEDefs::ErrCode::SUCCESS;
    };

    EXPECT_CALL(bleChip, sendRequest(IsBleCmdVariant(BLESPICommands::CommandId::GET_FW_VERSION)))
        .Times(1)
        .WillOnce(Invoke(bleGetFwVersionResponse));
    EXPECT_CALL(txListener, sendResponse).Times(0);

    EXPECT_CALL(screenVersionsReader, getVersions)
        .Times(1)
        .WillOnce(Return(ScreenDefs::ScreenErrCode::INVALID_STATE));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::VERSIONS, _, _))
        .With(Args<1, 2>(ElementsAreArray(resp)))
        .Times(1);

    versionsCmd.dataReceived(pData, dataLen);
}
