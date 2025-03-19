/*
 * FormatFlashCmd.hpp
 *
 *  Created on: Apr 17, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_FORMATFLASH_FORMATFLASHCMD_HPP_
#define APPCOMM_FORMATFLASH_FORMATFLASHCMD_HPP_

#include "AppCommCmd.hpp"
#include "IApplication.hpp"

class FormatFlashCmd : public AppCommCmd
{
public:
    FormatFlashCmd(IAppCommTxListener& appCommTxListener, IApplication& app);
    virtual ~FormatFlashCmd() = default;

    virtual void dataReceived(const uint8_t* pData, size_t dataLen);

private:
    IApplication& mApp;
};

#endif /* APPCOMM_FORMATFLASH_FORMATFLASHCMD_HPP_ */
