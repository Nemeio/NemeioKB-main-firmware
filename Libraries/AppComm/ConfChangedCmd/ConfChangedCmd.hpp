/*
 * ConfChangedCmd.hpp
 *
 *  Created on: Feb 28, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_CONFCHANGEDCMD_CONFCHANGEDCMD_HPP_
#define APPCOMM_CONFCHANGEDCMD_CONFCHANGEDCMD_HPP_

#include "AppCommCmd.hpp"
#include "IConfChangeListener.hpp"

class ConfChangedCmd : public AppCommCmd, public IConfChangeListener
{
public:
    ConfChangedCmd(IAppCommTxListener& appCommTxListener);
    virtual ~ConfChangedCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen);

    void configurationChanged(const Configuration* config);
};

#endif /* APPCOMM_CONFCHANGEDCMD_CONFCHANGEDCMD_HPP_ */
