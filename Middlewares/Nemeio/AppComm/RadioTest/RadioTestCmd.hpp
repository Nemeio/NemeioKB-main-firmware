/*
 * RadioTestCmd.hpp
 *
 *  Created on: 22 nov. 2021
 *      Author: eskoric
 */

#ifndef NEMEIO_APPCOMM_RADIOTEST_RADIOTESTCMD_HPP_
#define NEMEIO_APPCOMM_RADIOTEST_RADIOTESTCMD_HPP_

#include "AppCommCmd.hpp"
#include "IRadioTest.hpp"

class RadioTestCmd : public AppCommCmd
{
public:
    RadioTestCmd(IAppCommTxListener& appCommTxListener, IRadioTest& radioTest);
    virtual ~RadioTestCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen) override;

private:
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);

    IRadioTest& m_radioTest;
};

#endif /* NEMEIO_APPCOMM_RADIOTEST_RADIOTESTCMD_HPP_ */
