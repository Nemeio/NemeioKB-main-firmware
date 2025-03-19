/*
 * STM32I2C.h
 *
 *  Created on: 23 juin 2017
 *      Author: cgarnier
 */

#ifndef I2C_STM32I2C_STM32I2C_H_
#define I2C_STM32I2C_STM32I2C_H_

#include "iI2C.h"
#include "Signal.hpp"
#include "RecursiveMutex.hpp"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_i2c.h"

class STM32I2C : public iI2C
{
public:
    STM32I2C(I2C_TypeDef* instance);
    virtual ~STM32I2C();
    //II2CErrCode initI2C();
    II2CErrCode I2CWriteMem(uint16_t deviceAdd, uint16_t registerAdd, uint8_t txValue);
    II2CErrCode I2CWriteMem(uint16_t deviceAdd,
                            uint16_t registerAdd,
                            uint8_t rxData[],
                            uint16_t dataSize);
    II2CErrCode I2CReadMem(uint16_t deviceAdd,
                           uint16_t registerAdd,
                           uint8_t rxData[],
                           uint16_t dataSize);
    II2CErrCode masterToSlaveTransmit(uint16_t add, uint8_t txData[], uint16_t dataSize);
    II2CErrCode masterReceive(uint16_t add, uint8_t rxData[], uint16_t dataSize);
    void init(I2CConfig i2cConfig);

private:
    struct I2CSignals_t
    {
        Signal txCplt;
        Signal txRxCplt;
        Signal rxCplt;
    } I2CSignals;

    enum I2CInstanceIdx_t
    {
        I2C_INSTANCE_IDX_I2C1 = 0,
        I2C_INSTANCE_IDX_I2C2,
        I2C_INSTANCE_IDX_I2C3,
        I2C_INSTANCE_IDX_MAX_INDEX
    } I2CInstanceIdx;

    RecursiveMutex mMutex;
    I2C_HandleTypeDef mHI2C;
    I2C_TypeDef* mInstance;

    static constexpr uint8_t gNbI2CInstances = I2C_INSTANCE_IDX_MAX_INDEX;
    static bool sIsInstanceInitialized[gNbI2CInstances];

    uint32_t convertAddressSize(I2CAddressSize addressSize);
    uint32_t convertDualAddress(I2CDualAddressEnable dualAddress);
    uint32_t convertOwnAddress2Masks(I2COwnAddress2Masks ownAddress2Masks);
    uint32_t convertGenerallCallMode(I2CGeneralCallMode generalCallMode);
    uint32_t convertNoStretchMode(I2CNoStretchMode noStretchMode);

    uint32_t getIndexFromInstance(I2C_TypeDef* instance);
};

#endif /* I2C_STM32I2C_STM32I2C_H_ */
