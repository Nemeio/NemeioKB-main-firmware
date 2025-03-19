#pragma once

#include "AppCommCmd.hpp"
#include "ILayoutStorage.hpp"

class DeleteLayoutCmd : public AppCommCmd
{
public:
    DeleteLayoutCmd(AppCommDefs::AppCommCmd id,
                    IAppCommTxListener& appCommTxListener,
                    ILayoutStorage& storage);
    ~DeleteLayoutCmd() final = default;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;

private:
    ILayoutStorage& m_storage;
};
