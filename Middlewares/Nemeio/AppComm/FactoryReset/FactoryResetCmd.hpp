/*
 * FactoryResetCmd.hpp
 *
 *  Created on: Aug 26, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_APPCOMM_FACTORYRESET_FACTORYRESETCMD_HPP_
#define NEMEIO_APPCOMM_FACTORYRESET_FACTORYRESETCMD_HPP_

#include "AppCommCmd.hpp"
#include "IFactoryReset.hpp"
#include "ISystem.hpp"

class FactoryResetCmd : public AppCommCmd
{
public:
    FactoryResetCmd(IAppCommTxListener& appCommTxListener,
                    IFactoryReset& factoryReset,
                    ISystem& system);
    virtual ~FactoryResetCmd() = default;

    virtual void dataReceived(const uint8_t* pData, size_t dataLen);

private:
    static constexpr int32_t DELAY_BEFORE_SYSTEM_RESET_MS = 1500;
    IFactoryReset& mFactoryReset;
    ISystem& mSystem;

    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
    void actionBeforeSystemReset();
};

#endif /* NEMEIO_APPCOMM_FACTORYRESET_FACTORYRESETCMD_HPP_ */
