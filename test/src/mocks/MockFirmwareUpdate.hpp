#ifndef MOCKS_MOCKFIRMWAREUPDATE_HPP_
#define MOCKS_MOCKFIRMWAREUPDATE_HPP_

#include "gmock/gmock.h"
#include "IFirmwareUpdate.hpp"

class MockFirmwareUpdate : public IFirmwareUpdate
{
public:
    MOCK_METHOD(FirmwareUpdateDefs::ErrCode, initUpdate, (uint32_t len), (override));
    MOCK_METHOD(FirmwareUpdateDefs::ErrCode,
                writeFirmware,
                (const uint8_t* pData, std::size_t len),
                (override));
    MOCK_METHOD(FirmwareUpdateDefs::ErrCode, runUpdate, (), (override));
};

#endif /* MOCKS_MOCKFIRMWAREUPDATE_HPP_ */