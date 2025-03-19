#include "SendLayoutCmd.hpp"
#include "ApplicationConfig.h"
#include "AppCommUtils.hpp"

SendLayoutCmd::SendLayoutCmd(AppCommDefs::SendDataID id,
                             fs::IFileSystem& fileSystem,
                             etl::string_view directoryPath,
                             ILayoutWritersContainer& writersContainer)
    : SendDataListener(id)
    , m_writersContainer(writersContainer)
    , m_fileSystem(fileSystem)
    , m_directoryPath(directoryPath)
{
}

uint8_t SendLayoutCmd::doPrepareSend(const uint8_t* pHeader, size_t headerLen, size_t dataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(headerLen == uuid::SIZE)
    {
        ret = AppCommDefs::APPCOMMERR_UNEXPECTED;

        uuid::UuidView hash({reinterpret_cast<const std::byte*>(pHeader), headerLen});

        m_writer = m_writersContainer.createLayoutWriter(hash, m_fileSystem, "");
    }

    if(m_writer.has_value())
    {
        configuration::Ret confRet = m_writer.value().get().initWrite();
        ret = AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
    }

    return ret;
}

uint8_t SendLayoutCmd::doSendData(const uint8_t* pData, size_t len)
{
    if(!m_writer.has_value())
    {
        return AppCommDefs::APPCOMMERR_UNEXPECTED;
    }

    configuration::Ret confRet = m_writer.value().get().write(
        {reinterpret_cast<const std::byte*>(pData), len});

    return AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
}

uint8_t SendLayoutCmd::doTerminateSend()
{
    return AppCommDefs::APPCOMMERR_SUCCESS;
}

uint8_t SendLayoutCmd::doApplySend()
{
    if(!m_writer.has_value())
    {
        return AppCommDefs::APPCOMMERR_UNEXPECTED;
    }

    configuration::Ret confRet = m_writer.value().get().endWrite();

    if(confRet == configuration::Ret::SUCCESS)
    {
        confRet = m_writer.value().get().move(m_directoryPath);
    }

    return AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);
}

void SendLayoutCmd::doReset()
{
    if(m_writer.has_value())
    {
        m_writersContainer.resetLayoutWriter(m_writer.value().get().getId());

        m_writer.reset();
    }
}
