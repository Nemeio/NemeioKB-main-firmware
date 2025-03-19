#pragma once

#include "SendDataFileListener.hpp"
#include "etl/optional.h"
#include "utils.hpp"
#include "ILayoutWritersContainer.hpp"
#include "etl/string.h"

class SendLayoutCmd : public SendDataListener
{
public:
    explicit SendLayoutCmd(AppCommDefs::SendDataID id,
                           fs::IFileSystem& fileSystem,
                           etl::string_view directoryPath,
                           ILayoutWritersContainer& writersContainer);
    ~SendLayoutCmd() final = default;

private:
    PACK((struct Header { std::byte hash[uuid::SIZE]; }));

    ILayoutWritersContainer& m_writersContainer;
    fs::IFileSystem& m_fileSystem;
    etl::string<configuration::MAX_DIRECTORY_PATH_SIZE> m_directoryPath;
    etl::optional<etl::reference_wrapper<AbstractConfigurationFileWriter>> m_writer;

    uint8_t doPrepareSend(const uint8_t* pHeader, size_t headerLen, size_t dataLen) final;
    uint8_t doSendData(const uint8_t* pData, size_t len) final;
    uint8_t doTerminateSend() final;
    uint8_t doApplySend() final;
    void doReset() final;
};
