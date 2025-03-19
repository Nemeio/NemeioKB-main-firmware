/*
 * BatteryInformationCmd.hpp
 *
 *  Created on: Apr 26, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_BATTERYINFORMATION_BATTERYINFORMATIONCMD_HPP_
#define APPCOMM_BATTERYINFORMATION_BATTERYINFORMATIONCMD_HPP_

#include "AppCommCmd.hpp"
#include "utils.hpp"
#include "IBattery.hpp"
#include "WriteBuffer.hpp"

class BatteryInformationCmd : public AppCommCmd
{
public:
    BatteryInformationCmd(IAppCommTxListener& appCommTxListener, IBattery& battery);
    virtual ~BatteryInformationCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen);

private:
    PACK((struct RetCodeBatteryInformationResponse
          {
              AppCommDefs::AppCommErrCode errCode;

              RetCodeBatteryInformationResponse(AppCommDefs::AppCommErrCode errCode_)
                  : errCode(errCode_)
              {
              }
          }));

    PACK((struct BatteryInformationResponse
          {
              AppCommDefs::AppCommErrCode errCode;
              uint8_t stateOfChargePercent;
              uint16_t remainingCapacityMilliAh;
              uint32_t timeToFullSec;
              uint32_t timeToEmptySec;
              uint16_t cyclesHundredth;
              uint8_t agePercent;

              BatteryInformationResponse(AppCommDefs::AppCommErrCode errCode_,
                                         const BatteryInformation& batteryInformation)
                  : errCode(errCode_)
                  , stateOfChargePercent(batteryInformation.stateOfChargePercent)
                  , remainingCapacityMilliAh(HostToBE16(batteryInformation.remainingCapacityMilliAh))
                  , timeToFullSec(HostToBE32(batteryInformation.timeToFullSec))
                  , timeToEmptySec(HostToBE32(batteryInformation.timeToEmptySec))
                  , cyclesHundredth(HostToBE16(batteryInformation.cyclesHundredth))
                  , agePercent(batteryInformation.agePercent)
              {
              }
          }));

    static constexpr std::size_t MAX_RESPONSE_SIZE =
        std::max(sizeof(RetCodeBatteryInformationResponse), sizeof(BatteryInformationResponse));
    typedef WriteBuffer<uint8_t, MAX_RESPONSE_SIZE> RespBuffer;

    void getBatteryInformation(RespBuffer& respBuffer);
    void setRetCodeResponse(AppCommDefs::AppCommErrCode ret, RespBuffer& respBuffer);
    void setBatteryInformationResponse(AppCommDefs::AppCommErrCode ret,
                                       const BatteryInformation& batteryInformation,
                                       RespBuffer& respBuffer);

    AppCommDefs::AppCommErrCode convertBatteryRetCode(IBattery::RetCode batteryRetCode);

    IBattery& mBattery;
};

#endif /* APPCOMM_BATTERYINFORMATION_BATTERYINFORMATIONCMD_HPP_ */
