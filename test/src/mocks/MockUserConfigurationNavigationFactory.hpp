#pragma once
#include "gmock/gmock.h"
#include "IUserConfigurationNavigationFactory.hpp"

class MockUserConfigurationNavigationFactory : public IUserConfigurationNavigationFactory
{
public:
    MOCK_METHOD(IUserConfigurationNavigation&,
                createUserConfigurationNavigation,
                (uuid::UuidView id,
                 ConfigurationStorageContext& storageContext,
                 IApplication& application,
                 IConfigurationChangeNotifier& confChangeNotifier),
                (override));
};
