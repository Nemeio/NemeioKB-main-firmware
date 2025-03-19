/*
 * UpdateStatusCmd.hpp
 *
 *  Created on: 6 sept. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_APPCOMM_UPDATESTATUS_UPDATESTATUSCMD_HPP_
#define NEMEIO_APPCOMM_UPDATESTATUS_UPDATESTATUSCMD_HPP_

#include <cstdint>
#include "AppCommCmd.hpp"
#include "IAppCommTxListener/IAppCommTxListener.hpp"
#include "IUpdateStatusListener.hpp"

class UpdateStatusCmd : public AppCommCmd, public IUpdateStatusListener
{
public:
    UpdateStatusCmd(IAppCommTxListener& appCommTxListener);
    virtual ~UpdateStatusCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void notifyUpdateStatus(UpdateStatusDefs::UpdateStatus status) final;
};

#endif /* NEMEIO_APPCOMM_UPDATESTATUS_UPDATESTATUSCMD_HPP_ */
