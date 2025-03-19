#pragma once

#include "IUserConfigurationNavigation.hpp"
#include "ConfigurationStorageContext.hpp"
#include "IApplication.hpp"
#include "IConfigurationChangeNotifier.hpp"

class IUserConfigurationNavigationFactory
{
public:
    IUserConfigurationNavigationFactory() = default;
    virtual ~IUserConfigurationNavigationFactory() = default;

    virtual IUserConfigurationNavigation& createUserConfigurationNavigation(
        uuid::UuidView id,
        ConfigurationStorageContext& storageContext,
        IApplication& application,
        IConfigurationChangeNotifier& confChangeNotifier) = 0;
};