#pragma once

#include "SendDataFileListener.hpp"
#include "etl/optional.h"
#include "IUserConfigurationWritersContainer.hpp"
#include "utils.hpp"

class SendLayoutConfigurationCmd : public SendDataListener
{
public:
    explicit SendLayoutConfigurationCmd(
        AppCommDefs::SendDataID id,
        IUserConfigurationWritersContainer& userConfigurationWritersContainer);
    ~SendLayoutConfigurationCmd() final = default;

private:
    PACK((struct Header
          {
              std::byte userConfigurationId[uuid::SIZE];
              std::byte id[uuid::SIZE];
          }));

    IUserConfigurationWritersContainer& m_userConfigurationWritersContainer;
    etl::optional<etl::reference_wrapper<IConfigurationFileWriter>> m_writer;

    uint8_t doPrepareSend(const uint8_t* pHeader, size_t headerLen, size_t dataLen) final;
    uint8_t doSendData(const uint8_t* pData, size_t len) final;
    uint8_t doTerminateSend() final;
    uint8_t doApplySend() final;
    void doReset() final;
};
