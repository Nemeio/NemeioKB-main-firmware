#include "SendUserConfigurationCmd.hpp"
#include "ApplicationConfig.h"
#include "AppCommUtils.hpp"

SendUserConfigurationCmd::SendUserConfigurationCmd(
    AppCommDefs::SendDataID id,
    IUserConfigurationWritersContainer& userConfigurationWritersContainer,
    fs::IFileSystem& fileSystem,
    etl::string_view tempDirectoryPath)
    : SendDataListener(id)
    , m_userConfigurationWritersContainer(userConfigurationWritersContainer)
    , m_fileSystem(fileSystem)
    , m_tempDirectoryPath(tempDirectoryPath)
{
}

uint8_t SendUserConfigurationCmd::doPrepareSend(const uint8_t* pHeader,
                                                size_t headerLen,
                                                size_t dataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(m_writer.has_value())
    {
        m_userConfigurationWritersContainer.resetUserConfigurationWriter(
            m_writer.value().get().getId());

        m_writer.reset();
    }

    if(headerLen == uuid::SIZE)
    {
        ret = AppCommDefs::APPCOMMERR_UNEXPECTED;

        uuid::UuidView id = uuid::UuidView({reinterpret_cast<const std::byte*>(pHeader), headerLen});

        m_writer = m_userConfigurationWritersContainer
                       .createUserConfigurationWriter(id, m_fileSystem, m_tempDirectoryPath);
    }

    if(m_writer.has_value())
    {
        configuration::Ret confRet = m_writer.value().get().initWrite();
        ret = AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
    }

    return ret;
}

uint8_t SendUserConfigurationCmd::doSendData(const uint8_t* pData, size_t len)
{
    if(!m_writer.has_value())
    {
        return AppCommDefs::APPCOMMERR_UNEXPECTED;
    }

    configuration::Ret confRet = m_writer.value().get().write(
        {reinterpret_cast<const std::byte*>(pData), len});

    return AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
}

uint8_t SendUserConfigurationCmd::doTerminateSend()
{
    return AppCommDefs::APPCOMMERR_SUCCESS;
}

uint8_t SendUserConfigurationCmd::doApplySend()
{
    if(!m_writer.has_value())
    {
        return AppCommDefs::APPCOMMERR_UNEXPECTED;
    }

    configuration::Ret confRet = m_writer.value().get().endWrite();

    return AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
}

void SendUserConfigurationCmd::doReset()
{
    // Nothing to do
}