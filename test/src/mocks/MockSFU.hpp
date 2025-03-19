#ifndef MOCKS_MOCKSFU_HPP_
#define MOCKS_MOCKSFU_HPP_

#include "gmock/gmock.h"
#include "ISFU.hpp"

class MockSFU : public ISFU
{
public:
    MOCK_METHOD(bool, getDownloadAreaInfo, (SFU_FwImageFlashTypeDef * pArea), (override));
    MOCK_METHOD(bool, validateFirmware, (), (override));
    MOCK_METHOD(bool, isRecoveryUpdatePerformed, (), (override));
    MOCK_METHOD(void, resetBackupInfo, (), (override));
};

#endif /* MOCKS_MOCKSFU_HPP_ */