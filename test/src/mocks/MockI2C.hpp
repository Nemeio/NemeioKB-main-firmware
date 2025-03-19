#ifndef MOCKS_MOCKI2C_HPP_
#define MOCKS_MOCKI2C_HPP_

#include "gmock/gmock.h"
#include "iI2C.h"

class MockI2C : public iI2C
{
public:
    MOCK_METHOD(II2CErrCode,
                I2CWriteMem,
                (uint16_t deviceAdd, uint16_t registerAdd, uint8_t txValue),
                (override));
    MOCK_METHOD(II2CErrCode,
                I2CWriteMem,
                (uint16_t deviceAdd, uint16_t registerAdd, uint8_t rxData[], uint16_t dataSize),
                (override));
    MOCK_METHOD(II2CErrCode,
                I2CReadMem,
                (uint16_t deviceAdd, uint16_t registerAdd, uint8_t rxData[], uint16_t dataSize),
                (override));
    MOCK_METHOD(II2CErrCode,
                masterToSlaveTransmit,
                (uint16_t add, uint8_t txData[], uint16_t dataSize),
                (override));
    MOCK_METHOD(II2CErrCode,
                masterReceive,
                (uint16_t add, uint8_t rxData[], uint16_t dataSize),
                (override));
};

#endif /* MOCKS_MOCKI2C_HPP_ */