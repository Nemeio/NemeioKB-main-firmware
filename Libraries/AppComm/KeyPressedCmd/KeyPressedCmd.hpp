/*
 * KeyPressedCmd.hpp
 *
 *  Created on: Mar 4, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_KEYPRESSEDCMD_KEYPRESSEDCMD_HPP_
#define APPCOMM_KEYPRESSEDCMD_KEYPRESSEDCMD_HPP_

#include "AppCommCmd.hpp"
#include "IKeyDataSender.hpp"

class KeyPressedCmd : public AppCommCmd, public IKeyDataSender
{
public:
    KeyPressedCmd(IAppCommTxListener& appCommTxListener);
    virtual ~KeyPressedCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen);

    void sendKeyData(uint8_t* pData, size_t len);
};

#endif /* APPCOMM_KEYPRESSEDCMD_KEYPRESSEDCMD_HPP_ */
