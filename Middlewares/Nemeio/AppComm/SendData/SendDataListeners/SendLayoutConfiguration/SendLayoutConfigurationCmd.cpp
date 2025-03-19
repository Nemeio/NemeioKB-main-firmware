#include "SendLayoutConfigurationCmd.hpp"
#include "ApplicationConfig.h"
#include "AppCommUtils.hpp"

SendLayoutConfigurationCmd::SendLayoutConfigurationCmd(
    AppCommDefs::SendDataID id,
    IUserConfigurationWritersContainer& userConfigurationWritersContainer)
    : SendDataListener(id)
    , m_userConfigurationWritersContainer(userConfigurationWritersContainer)
{
}

uint8_t SendLayoutConfigurationCmd::doPrepareSend(const uint8_t* pHeader,
                                                  size_t headerLen,
                                                  size_t dataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(headerLen == sizeof(Header))
    {
        ret = AppCommDefs::APPCOMMERR_NOT_FOUND;

        const Header& header = *reinterpret_cast<const Header*>(pHeader);

        uuid::UuidView userConfigurationId(
            {reinterpret_cast<const std::byte*>(header.userConfigurationId),
             sizeof(header.userConfigurationId)});
        uuid::UuidView layoutConfigurationId(
            {reinterpret_cast<const std::byte*>(header.id), sizeof(header.userConfigurationId)});

        auto userConfigurationWriter = m_userConfigurationWritersContainer
                                           .getUserConfigurationWriter(userConfigurationId);

        if(userConfigurationWriter.has_value())
        {
            m_writer.emplace(userConfigurationWriter.value().get().createLayoutConfigurationWriter(
                layoutConfigurationId));
        }
    }

    if(m_writer.has_value())
    {
        configuration::Ret confRet = m_writer.value().get().initWrite();
        ret = AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
    }

    return ret;
}

uint8_t SendLayoutConfigurationCmd::doSendData(const uint8_t* pData, size_t len)
{
    if(!m_writer.has_value())
    {
        return AppCommDefs::APPCOMMERR_UNEXPECTED;
    }

    configuration::Ret confRet = m_writer.value().get().write(
        {reinterpret_cast<const std::byte*>(pData), len});

    return AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
}

uint8_t SendLayoutConfigurationCmd::doTerminateSend()
{
    return AppCommDefs::APPCOMMERR_SUCCESS;
}

uint8_t SendLayoutConfigurationCmd::doApplySend()
{
    if(!m_writer.has_value())
    {
        return AppCommDefs::APPCOMMERR_UNEXPECTED;
    }

    configuration::Ret confRet = m_writer.value().get().endWrite();

    return AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
}

void SendLayoutConfigurationCmd::doReset()
{
    m_writer.reset();
}
