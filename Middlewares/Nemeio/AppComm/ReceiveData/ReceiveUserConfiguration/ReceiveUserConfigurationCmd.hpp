#pragma once

#include "ReceiveDataFileListener.hpp"
#include "IUserConfigurationStorage.hpp"

class ReceiveUserConfigurationCmd : public ReceiveDataFileListener
{
public:
    explicit ReceiveUserConfigurationCmd(AppCommDefs::ReceiveDataID id,
                                         IUserConfigurationStorage& storage);
    ~ReceiveUserConfigurationCmd() final = default;

private:
    IUserConfigurationStorage& m_storage;

    uint8_t doPrepareReceive(const uint8_t* pHeader,
                             size_t headerLen,
                             std::size_t& retDataLen) final;
    void doReceiveFileDone() final;
};
