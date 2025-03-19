/*
 * SetConfiguration.cpp
 *
 *  Created on: Feb 20, 2019
 *      Author: Labo
 */

#include "ApplyConfigurationCmd.hpp"
#include "AppCommUtils.hpp"

ApplyConfigurationCmd::ApplyConfigurationCmd(IAppCommTxListener& appCommTxListener,
                                             IConfigurationSwitcher& configurationSwitcher)
    : AppCommCmd(AppCommDefs::AppCommCmd::APPLYLAYOUTCONFIG, appCommTxListener)
    , m_configurationSwitcher(configurationSwitcher)
{
}

void ApplyConfigurationCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(pData != nullptr && dataLen == uuid::SIZE)
    {
        uuid::UuidView id(
            etl::array_view<const std::byte>{reinterpret_cast<const std::byte*>(pData), dataLen});
        configuration::Ret selectRet = m_configurationSwitcher.selectLayoutConfiguration(id);
        ret = AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(selectRet);
    }

    transmitResponse(&ret, sizeof(ret));
}