/*
 * UpdateStatusCmd.cpp
 *
 *  Created on: 6 sept. 2021
 *      Author: thomas
 */

#include "UpdateStatusCmd.hpp"
#include "utils.hpp"

PACK((struct updateStatus
      {
          uint8_t component;
          uint8_t notificationType;
          uint8_t payload;
      }));

UpdateStatusCmd::UpdateStatusCmd(IAppCommTxListener& appComTxListener)
    : AppCommCmd(AppCommDefs::AppCommCmd::UPDATESTATUS, appComTxListener)
{
}

void UpdateStatusCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
}

void UpdateStatusCmd::notifyUpdateStatus(UpdateStatusDefs::UpdateStatus status)
{
    updateStatus updateStatusCmd = {
        .component = static_cast<uint8_t>(status.mComponent),
        .notificationType = static_cast<uint8_t>(status.mType),
        .payload = status.mPayload,
    };

    transmitCmd(reinterpret_cast<uint8_t*>(&updateStatusCmd), sizeof(updateStatusCmd));
}
