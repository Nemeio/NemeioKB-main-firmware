/*
 * SystemResetCmd.hpp
 *
 *  Created on: Dec 17, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_APPCOMM_SYSTEMRESET_SYSTEMRESETCMD_HPP_
#define NEMEIO_APPCOMM_SYSTEMRESET_SYSTEMRESETCMD_HPP_

#include "AppCommCmd.hpp"
#include "ISystem.hpp"

class SystemResetCmd : public AppCommCmd
{
public:
    SystemResetCmd(IAppCommTxListener& appCommTxListener, ISystem& system);
    virtual ~SystemResetCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen);

private:
    ISystem& mSystem;

    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};

#endif /* NEMEIO_APPCOMM_SYSTEMRESET_SYSTEMRESETCMD_HPP_ */
