/*
 * SetCommModeCmd.hpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_SETCOMMMODE_SETCOMMMODECMD_HPP_
#define APPCOMM_SETCOMMMODE_SETCOMMMODECMD_HPP_

#include "AppComm.hpp"
#include "KeyProtocolManager.hpp"

class SetCommModeCmd : public AppCommCmd
{
public:
    SetCommModeCmd(IAppCommTxListener& appCommTxListener, KeyProtocolManager& keyProtocolManager);
    virtual ~SetCommModeCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen);

private:
    KeyProtocolManager& mKeyProtocolManager;

    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};

#endif /* APPCOMM_SETCOMMMODE_SETCOMMMODECMD_HPP_ */
