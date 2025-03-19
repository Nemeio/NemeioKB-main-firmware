#pragma once

#include "AppCommCmd.hpp"
#include "IUserConfigurationStorage.hpp"

class DeleteUserConfigurationCmd : public AppCommCmd
{
public:
    DeleteUserConfigurationCmd(AppCommDefs::AppCommCmd id,
                               IAppCommTxListener& appCommTxListener,
                               IUserConfigurationStorage& storage);
    ~DeleteUserConfigurationCmd() final = default;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;

private:
    IUserConfigurationStorage& m_storage;
};
