#ifndef MOCKS_MOCKSPI_HPP_
#define MOCKS_MOCKSPI_HPP_

#include "gmock/gmock.h"
#include "iSPI.h"

class MockSPI : public iSPI
{
public:
    MOCK_METHOD(ISPIErrCode,
                transmit,
                (const uint8_t txData[], size_t size, iChipSelector& chipSelector, bool bEndTransfer),
                (override));
    MOCK_METHOD(
        ISPIErrCode,
        receive,
        (uint8_t rxData[], size_t size, iChipSelector& chipSelector, bool bEndTransfer, bool bDMA),
        (override));
    MOCK_METHOD(ISPIErrCode,
                transmitReceive,
                (const uint8_t txData[],
                 uint8_t rxData[],
                 size_t size,
                 iChipSelector& chipSelector,
                 bool bEndTransfer),
                (override));
    MOCK_METHOD(ISPIErrCode, init, (SPIConfig spiConfig), (override));
    MOCK_METHOD(ISPIErrCode, deinit, (), (override));
    MOCK_METHOD(ISPIErrCode, endTransfer, (iChipSelector & chipSelector), (override));
    MOCK_METHOD(ISPIErrCode, clearMosi, (), (override));
};

#endif /* MOCKS_MOCKSPI_HPP_ */