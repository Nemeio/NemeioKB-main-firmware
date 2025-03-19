#pragma once
#include "gmock/gmock.h"
#include "ILayoutStorage.hpp"

class MockLayoutStorage : public ILayoutStorage
{
public:
    MOCK_METHOD(configuration::Ret, deleteLayout, (uuid::UuidView id), (override));
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(configuration::Ret,
                getLayout,
                (uuid::UuidView hash, etl::optional<configuration::Layout>& layout),
                (override));
    MOCK_METHOD((etl::optional<etl::reference_wrapper<Iterator>>), createIterator, (), (override));
    MOCK_METHOD(void, deleteIterator, (Iterator & iterator), (override));
    MOCK_METHOD(fs::IFileSystem&, getFileSystem, (), (override));
};
