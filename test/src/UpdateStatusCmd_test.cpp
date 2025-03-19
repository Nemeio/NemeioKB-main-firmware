#include "gtest/gtest.h"

#include "UpdateStatusCmd.hpp"
#include "MockAppCommTxListener.hpp"

using testing::_;
using testing::Args;
using testing::ElementsAreArray;
using ::testing::Return;

class UpdateStatusCmdTest : public ::testing::Test
{
protected:
    UpdateStatusCmdTest()
        : updateStatusCmd(txListener)
    {
    }

    MockAppCommTxListener txListener;
    UpdateStatusCmd updateStatusCmd;
};

TEST_F(UpdateStatusCmdTest, UpdateProgress)
{
    std::vector<uint8_t> cmd;
    cmd.push_back(0x05);
    cmd.push_back(0x02);
    cmd.push_back(50);

    UpdateStatusDefs::UpdateStatus
        status(Identification::ModuleId::BLE,
               UpdateStatusDefs::UpdateStatusNotificationType::UPDATE_PROGRESS,
               50);

    EXPECT_CALL(txListener, sendCmd(AppCommDefs::AppCommCmd::UPDATESTATUS, _, _))
        .With(Args<1, 2>(ElementsAreArray(cmd)))
        .Times(1);
    updateStatusCmd.notifyUpdateStatus(status);
}

TEST_F(UpdateStatusCmdTest, UpdateFailed)
{
    std::vector<uint8_t> cmd;
    cmd.push_back(0x05);
    cmd.push_back(0x01);
    cmd.push_back(0);

    UpdateStatusDefs::UpdateStatus
        status(Identification::ModuleId::BLE,
               UpdateStatusDefs::UpdateStatusNotificationType::UPDATE_FAILED,
               0);

    EXPECT_CALL(txListener, sendCmd(AppCommDefs::AppCommCmd::UPDATESTATUS, _, _))
        .With(Args<1, 2>(ElementsAreArray(cmd)))
        .Times(1);
    updateStatusCmd.notifyUpdateStatus(status);
}
