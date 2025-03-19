#pragma once

#include "AppCommCmd.hpp"
#include "etl/observer.h"
#include "IConfigurationSwitcher.hpp"
#include "utils.hpp"

class ApplyUserConfigurationCmd : public AppCommCmd
{
public:
    ApplyUserConfigurationCmd(IAppCommTxListener& appCommTxListener,
                              IConfigurationSwitcher& configurationSwitcher);
    ~ApplyUserConfigurationCmd() final = default;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;

private:
    PACK((struct Args
          {
              std::byte userConfigurationId[uuid::SIZE];
              std::byte layoutConfigurationId[uuid::SIZE];
          }));

    IConfigurationSwitcher& m_configurationSwitcher;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};
