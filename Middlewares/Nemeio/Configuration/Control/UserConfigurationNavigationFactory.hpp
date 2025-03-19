#pragma once

#include "IUserConfigurationNavigationFactory.hpp"
#include "UserConfigurationNavigation.hpp"

class UserConfigurationNavigationFactory : public IUserConfigurationNavigationFactory
{
public:
    UserConfigurationNavigationFactory() = default;
    ~UserConfigurationNavigationFactory() final = default;

    // This method will delete the previous created object
    IUserConfigurationNavigation& createUserConfigurationNavigation(
        uuid::UuidView id,
        ConfigurationStorageContext& storageContext,
        IApplication& application,
        IConfigurationChangeNotifier& confChangeNotifier) final
    {
        m_userConfigurationNavigation.emplace(id, storageContext, application, confChangeNotifier);

        return m_userConfigurationNavigation.value();
    }

private:
    etl::optional<UserConfigurationNavigation> m_userConfigurationNavigation;
};