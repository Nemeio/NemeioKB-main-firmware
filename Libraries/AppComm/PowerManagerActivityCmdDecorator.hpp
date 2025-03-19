/*
 * PowerManagedCmd.hpp
 *
 *  Created on: 20 août 2021
 *      Author: thomas
 */

#ifndef NEMEIO_APPCOMM_POWERMANAGERACTIVITYCMDDECORATOR_HPP_
#define NEMEIO_APPCOMM_POWERMANAGERACTIVITYCMDDECORATOR_HPP_

#include "AppCommCmd.hpp"
#include "IPowerManager.hpp"

class PowerManagerActivityCmdDecorator : public IAppCommCmd
{
public:
    PowerManagerActivityCmdDecorator(IPowerManager& powerManager, AppCommCmd& cmd);
    virtual ~PowerManagerActivityCmdDecorator() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    AppCommDefs::AppCommCmd getCmdId() const final;

private:
    IPowerManager& mPowerManager;
    AppCommCmd& mAppCommCmd;
};

#endif /* NEMEIO_APPCOMM_POWERMANAGERACTIVITYCMDDECORATOR_HPP_ */
