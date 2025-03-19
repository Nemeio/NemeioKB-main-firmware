#pragma once
#include "gmock/gmock.h"
#include "IUserConfigurationNavigation.hpp"

class MockUserConfigurationNavigation : public IUserConfigurationNavigation
{
public:
    MOCK_METHOD(configuration::Ret, enableCustom, (), (override));
    MOCK_METHOD(configuration::Ret, disableCustom, (), (override));
    MOCK_METHOD(configuration::Ret, applyFirst, (), (override));
    MOCK_METHOD(configuration::Ret, applyNext, (SwitchDirection direction), (override));
    MOCK_METHOD(configuration::Ret, applyId, (uuid::UuidView id), (override));

    MOCK_METHOD(etl::optional<uuid::UuidView>, getCurrentUserConfigurationId, (), (const override));
};
