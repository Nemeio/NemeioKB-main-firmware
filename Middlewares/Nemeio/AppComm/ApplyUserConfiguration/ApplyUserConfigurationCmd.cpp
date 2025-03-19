#include "ApplyUserConfigurationCmd.hpp"
#include "AppCommUtils.hpp"

ApplyUserConfigurationCmd::ApplyUserConfigurationCmd(IAppCommTxListener& appCommTxListener,
                                                     IConfigurationSwitcher& configurationSwitcher)
    : AppCommCmd(AppCommDefs::AppCommCmd::APPLYUSERCONFIGURATION, appCommTxListener)
    , m_configurationSwitcher(configurationSwitcher)
{
}

void ApplyUserConfigurationCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if((dataLen == sizeof(Args)) && (pData != nullptr))
    {
        auto& args = *reinterpret_cast<const Args*>(pData);

        configuration::Ret storageRet = m_configurationSwitcher.selectUserConfiguration(
            uuid::UuidView(args.userConfigurationId), uuid::UuidView(args.layoutConfigurationId));

        retCode = AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(storageRet);
    }

    replyRetCode(retCode);
}

void ApplyUserConfigurationCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
