/*
 * AppCommCmd.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_APPCOMMCMD_APPCOMMCMD_HPP_
#define APPCOMM_APPCOMMCMD_APPCOMMCMD_HPP_

#include <cstdint>
#include "AppCommDefs.hpp"
#include <IAppCommTxListener/IAppCommTxListener.hpp>
#include "IAppCommCmd.hpp"

class AppCommCmd : public IAppCommCmd
{
public:
    AppCommCmd(AppCommDefs::AppCommCmd cmdId, IAppCommTxListener& appCommTxListener);
    virtual ~AppCommCmd() = default;

    virtual void dataReceived(const uint8_t* pData, size_t dataLen) = 0;
    AppCommDefs::AppCommCmd getCmdId() const;

protected:
    void transmitCmd(const uint8_t* pData, size_t len);
    void transmitResponse(const uint8_t* pData, size_t len);

private:
    AppCommDefs::AppCommCmd mCmdId;
    IAppCommTxListener& mAppCommTxListener;
};

inline AppCommDefs::AppCommCmd AppCommCmd::getCmdId() const
{
    return mCmdId;
}

#endif /* APPCOMM_APPCOMMCMD_APPCOMMCMD_HPP_ */
