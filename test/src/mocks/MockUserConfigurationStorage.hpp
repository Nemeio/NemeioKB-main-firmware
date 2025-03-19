#pragma once
#include "gmock/gmock.h"
#include "IUserConfigurationStorage.hpp"

class MockUserConfigurationStorage : public IUserConfigurationStorage
{
public:
    MOCK_METHOD(configuration::Ret, deleteUserConfiguration, (uuid::UuidView id), (override));
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(bool, isExistingUserConfiguration, (uuid::UuidView id), (override));
    MOCK_METHOD(configuration::Ret,
                getUserConfiguration,
                (uuid::UuidView id,
                 etl::optional<configuration::UserConfiguration>& userConfiguration),
                (override));
    MOCK_METHOD(configuration::Ret,
                getLayoutConfiguration,
                (uuid::UuidView userConfigurationId,
                 uuid::UuidView layoutConfigurationId,
                 etl::optional<configuration::LayoutConfiguration>& layoutConfiguration),
                (override));
    MOCK_METHOD((etl::optional<etl::reference_wrapper<Iterator>>), createIterator, (), (override));
    MOCK_METHOD(void, deleteIterator, (Iterator & iterator), (override));
    MOCK_METHOD(void,
                getUserConfigurationPath,
                (uuid::UuidView id, embedded_ostream& retPath),
                (const override));
    MOCK_METHOD(fs::IFileSystem&, getFileSystem, (), (override));
};
