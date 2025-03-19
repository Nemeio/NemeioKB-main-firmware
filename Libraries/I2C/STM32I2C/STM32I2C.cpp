/*
 * STM32I2C.cpp
 *
 *  Created on: 23 juin 2017
 *      Author: cgarnier
 */

#include "../STM32I2C/STM32I2C.h"
#include "timing_macros.h"
#include "AutoLock.h"
#include "cmsis_os.h"
#include "assertTools.h"

bool STM32I2C::sIsInstanceInitialized[gNbI2CInstances] = {0};

static const uint32_t I2C_TIMEOUT = 10000;


STM32I2C::STM32I2C(I2C_TypeDef* instance)
    : iI2C()
    , mInstance(instance)
{
    memset(&mHI2C, 0, sizeof(mHI2C));
}

void STM32I2C::init(I2CConfig i2cConfig)
{
    TRACE_FUNC_ENTRY();

    uint32_t instanceIndex = getIndexFromInstance(mInstance);

    ASSERT(instanceIndex < sizeof(sIsInstanceInitialized) / sizeof(*sIsInstanceInitialized));
    if(!sIsInstanceInitialized[instanceIndex])
    {
        mHI2C.Instance = mInstance;
        mHI2C.Init.Timing = i2cConfig.timing;
        mHI2C.Init.OwnAddress1 = 0;
        mHI2C.Init.AddressingMode = convertAddressSize(i2cConfig.addressSize);
        mHI2C.Init.DualAddressMode = convertDualAddress(i2cConfig.dualAdress);
        mHI2C.Init.OwnAddress2 = 0;
        mHI2C.Init.OwnAddress2Masks = convertOwnAddress2Masks(i2cConfig.ownAddresse2Masks);
        mHI2C.Init.GeneralCallMode = convertGenerallCallMode(i2cConfig.generalCallMode);
        mHI2C.Init.NoStretchMode = convertNoStretchMode(i2cConfig.noStretchMode);
        HAL_I2C_Init(&mHI2C);
    }

    TRACE_FUNC_EXIT();
}

STM32I2C::~STM32I2C()
{
}

II2CErrCode STM32I2C::I2CWriteMem(uint16_t deviceAdd, uint16_t registerAdd, uint8_t txValue)
{
    AutoLock autoLock(mMutex);

    II2CErrCode err = II2C_OK;
    if(sizeof(txValue) > UINT8_MAX)
    {
        err = II2C_ERROR_INVALID_PARAMETER;
    }
    if(HAL_OK
       != HAL_I2C_Mem_Write(&mHI2C,
                            deviceAdd << 1,
                            registerAdd,
                            I2C_MEMADD_SIZE_8BIT,
                            &txValue,
                            sizeof(txValue),
                            I2C_TIMEOUT))
    {
        err = II2C_ERROR;
    }
    return err;
}

II2CErrCode STM32I2C::I2CWriteMem(uint16_t deviceAdd,
                                  uint16_t registerAdd,
                                  uint8_t rxData[],
                                  uint16_t dataSize)
{
    AutoLock autoLock(mMutex);

    II2CErrCode err = II2C_OK;
    if(sizeof(dataSize) > UINT8_MAX)
    {
        err = II2C_ERROR_INVALID_PARAMETER;
    }
    if(HAL_OK
       != HAL_I2C_Mem_Write(&mHI2C,
                            deviceAdd << 1,
                            registerAdd,
                            I2C_MEMADD_SIZE_8BIT,
                            rxData,
                            dataSize,
                            I2C_TIMEOUT))
    {
        err = II2C_ERROR;
    }
    return err;
}

II2CErrCode STM32I2C::I2CReadMem(uint16_t deviceAdd,
                                 uint16_t registerAdd,
                                 uint8_t rxData[],
                                 uint16_t dataSize)
{
    AutoLock autoLock(mMutex);

    II2CErrCode err = II2C_OK;
    if(dataSize > UINT8_MAX)
    {
        err = II2C_ERROR_INVALID_PARAMETER;
    }
    if(HAL_OK
       != HAL_I2C_Mem_Read(&mHI2C,
                           deviceAdd << 1,
                           registerAdd,
                           I2C_MEMADD_SIZE_8BIT,
                           rxData,
                           dataSize,
                           I2C_TIMEOUT))
    {
        err = II2C_ERROR;
    }
    return err;
}

II2CErrCode STM32I2C::masterToSlaveTransmit(uint16_t add, uint8_t txData[], uint16_t dataSize)
{
    AutoLock autoLock(mMutex);

    II2CErrCode err = II2C_OK;
    if(dataSize > UINT8_MAX)
    {
        err = II2C_ERROR_INVALID_PARAMETER;
    }

    if(HAL_OK != HAL_I2C_Master_Transmit(&mHI2C, add << 1, txData, dataSize, I2C_TIMEOUT))
    {
        err = II2C_ERROR;
    }
    return err;
}

II2CErrCode STM32I2C::masterReceive(uint16_t add, uint8_t rxData[], uint16_t dataSize)
{
    AutoLock autoLock(mMutex);

    II2CErrCode err = II2C_OK;
    if(dataSize > UINT8_MAX)
    {
        err = II2C_ERROR_INVALID_PARAMETER;
    }
    if(HAL_OK != HAL_I2C_Master_Receive(&mHI2C, add << 1, rxData, dataSize, I2C_TIMEOUT))
    {
        err = II2C_ERROR;
    }
    return err;
}

uint32_t STM32I2C::convertAddressSize(I2CAddressSize addressSize)
{
    switch(addressSize)
    {
    case II2C_ADDRESSINGMODE_7BIT:
        return I2C_ADDRESSINGMODE_7BIT;
    case II2C_ADDRESSINGMODE_10BIT:
        return I2C_ADDRESSINGMODE_10BIT;
    default:
        return I2C_ADDRESSINGMODE_7BIT;
    }
}
uint32_t STM32I2C::convertDualAddress(I2CDualAddressEnable dualAddress)
{
    switch(dualAddress)
    {
    case II2C_DUALADDRESS_DISABLE:
        return I2C_DUALADDRESS_DISABLE;
    case II2C_DUALADDRESS_ENABLE:
        return II2C_DUALADDRESS_ENABLE;
    default:
        return I2C_DUALADDRESS_DISABLE;
    }
}
uint32_t STM32I2C::convertOwnAddress2Masks(I2COwnAddress2Masks ownAddress2Masks)
{
    switch(ownAddress2Masks)
    {
    case II2C_OA2_MASK01:
        return I2C_OA2_MASK01;
    case II2C_OA2_MASK02:
        return I2C_OA2_MASK02;
    case II2C_OA2_MASK03:
        return I2C_OA2_MASK03;
    case II2C_OA2_MASK04:
        return I2C_OA2_MASK04;
    case II2C_OA2_MASK05:
        return I2C_OA2_MASK05;
    case II2C_OA2_MASK06:
        return I2C_OA2_MASK06;
    case II2C_OA2_MASK07:
        return I2C_OA2_MASK07;
    case II2C_OA2_NOMASK:
        return I2C_OA2_NOMASK;

    default:
        return I2C_OA2_NOMASK;
    }
}
uint32_t STM32I2C::convertGenerallCallMode(I2CGeneralCallMode generalCallMode)
{
    switch(generalCallMode)
    {
    case II2C_GENERALCALL_DISABLE:
        return I2C_GENERALCALL_DISABLE;
    case II2C_GENERALCALL_ENABLE:
        return I2C_GENERALCALL_ENABLE;
    default:
        return I2C_GENERALCALL_DISABLE;
    }
}
uint32_t STM32I2C::convertNoStretchMode(I2CNoStretchMode noStretchMode)
{
    switch(noStretchMode)
    {
    case II2C_NOSTRETCH_DISABLE:
        return I2C_NOSTRETCH_DISABLE;
    case II2C_NOSTRETCH_ENABLE:
        return I2C_NOSTRETCH_ENABLE;
    default:
        return I2C_NOSTRETCH_ENABLE;
    }
}

uint32_t STM32I2C::getIndexFromInstance(I2C_TypeDef* instance)
{
    if(I2C2 == instance)
    {
        return I2C_INSTANCE_IDX_I2C2;
    }
    else if(I2C3 == instance)
    {
        return I2C_INSTANCE_IDX_I2C3;
    }
    else
    {
        return I2C_INSTANCE_IDX_I2C1;
    }
}
