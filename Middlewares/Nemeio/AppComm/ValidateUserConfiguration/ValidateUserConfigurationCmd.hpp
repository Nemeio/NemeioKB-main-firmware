#pragma once

#include "AppCommCmd.hpp"
#include "etl/observer.h"
#include "IUserConfigurationWritersContainer.hpp"
#include "IUserConfigurationStorage.hpp"
#include "UserConfigurationUpdateObserver.hpp"

inline constexpr size_t USER_CONFIGURATION_UPDATE_MAX_OBSERVERS = 1;

class ValidateUserConfigurationCmd
    : public AppCommCmd,
      public etl::observable<UserConfigurationUpdateObserver, USER_CONFIGURATION_UPDATE_MAX_OBSERVERS>
{
public:
    ValidateUserConfigurationCmd(
        AppCommDefs::AppCommCmd id,
        IAppCommTxListener& appCommTxListener,
        IUserConfigurationWritersContainer& userConfigurationWritersContainer,
        etl::string_view validDirectoryPath,
        IUserConfigurationStorage& storage);
    ~ValidateUserConfigurationCmd() final = default;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;

private:
    IUserConfigurationWritersContainer& m_userConfigurationWritersContainer;
    etl::string<configuration::MAX_DIRECTORY_PATH_SIZE> m_validDirectoryPath;
    IUserConfigurationStorage& m_storage;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
    void cleanupUserConfigurations(uuid::UuidView idToKeep);
};
