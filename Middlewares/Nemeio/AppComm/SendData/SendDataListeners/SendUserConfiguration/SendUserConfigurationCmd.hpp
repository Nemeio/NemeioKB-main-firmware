#pragma once

#include "SendDataFileListener.hpp"
#include "etl/optional.h"
#include "IUserConfigurationStorage.hpp"
#include "ApplicationConfig.h"
#include "AppCommUtils.hpp"
#include "IUserConfigurationWritersContainer.hpp"

class SendUserConfigurationCmd : public SendDataListener
{
public:
    explicit SendUserConfigurationCmd(
        AppCommDefs::SendDataID id,
        IUserConfigurationWritersContainer& userConfigurationWritersContainer,
        fs::IFileSystem& fileSystem,
        etl::string_view tempDirectoryPath);
    ~SendUserConfigurationCmd() final = default;

private:
    IUserConfigurationWritersContainer& m_userConfigurationWritersContainer;
    fs::IFileSystem& m_fileSystem;
    etl::string<configuration::MAX_DIRECTORY_PATH_SIZE> m_tempDirectoryPath;
    etl::optional<etl::reference_wrapper<AbstractUserConfigurationWriter>> m_writer;

    uint8_t doPrepareSend(const uint8_t* pHeader, size_t headerLen, size_t dataLen) final;
    uint8_t doSendData(const uint8_t* pData, size_t len) final;
    uint8_t doTerminateSend() final;
    uint8_t doApplySend() final;
    void doReset() final;
};