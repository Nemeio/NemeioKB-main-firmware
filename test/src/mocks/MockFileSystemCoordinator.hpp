#ifndef MOCKS_MOCKFILESYSTEMCOORDINATOR_HPP_
#define MOCKS_MOCKFILESYSTEMCOORDINATOR_HPP_

#include "gmock/gmock.h"
#include "IFileSystemCoordinator.hpp"

class MockFileSystemCoordinator : public fscommon::IFileSystemCoordinator
{
public:
    MOCK_METHOD(void, notifyFSRequest, (), (override));
    MOCK_METHOD(void, notifyWrite, (), (override));
};

#endif /* MOCKS_MOCKFILESYSTEMCOORDINATOR_HPP_ */
