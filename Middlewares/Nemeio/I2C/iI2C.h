/*
 * iI2C.h
 *
 *  Created on: 23 juin 2017
 *      Author: cgarnier
 */

#ifndef NEMEIO_I2C_II2C_II2C_H_
#define NEMEIO_I2C_II2C_II2C_H_

#include <stdint.h>
#include <stddef.h>

typedef enum
{
    II2C_OK,
    II2C_ERROR,
    II2C_BUSY,
    II2C_TIMEOUT,
    II2C_ERROR_INVALID_PARAMETER
} II2CErrCode;


typedef enum
{
    UNDEFINED,
    RED,
    GREEN,
    BLUE,
    WHITE,
} Colors;


typedef enum I2CAddressingSize_t
{
    II2C_ADDRESSINGMODE_7BIT,
    II2C_ADDRESSINGMODE_10BIT
} I2CAddressSize;

typedef enum I2CDualAddressEnable_t
{
    II2C_DUALADDRESS_DISABLE,
    II2C_DUALADDRESS_ENABLE
} I2CDualAddressEnable;

typedef enum I2COwnAddress2Masks_t
{
    II2C_OA2_NOMASK,
    II2C_OA2_MASK01,
    II2C_OA2_MASK02,
    II2C_OA2_MASK03,
    II2C_OA2_MASK04,
    II2C_OA2_MASK05,
    II2C_OA2_MASK06,
    II2C_OA2_MASK07,
} I2COwnAddress2Masks;

typedef enum I2CGeneralCallMode_t
{
    II2C_GENERALCALL_DISABLE,
    II2C_GENERALCALL_ENABLE
} I2CGeneralCallMode;

typedef enum I2CNoStretchMode_t
{
    II2C_NOSTRETCH_DISABLE,
    II2C_NOSTRETCH_ENABLE
} I2CNoStretchMode;

typedef struct I2CConfig_t
{
    uint32_t timing;

    I2CAddressSize addressSize;

    I2CDualAddressEnable dualAdress;

    I2COwnAddress2Masks ownAddresse2Masks;

    I2CGeneralCallMode generalCallMode;

    I2CNoStretchMode noStretchMode;

} I2CConfig;

class iI2C
{
public:
    iI2C() = default;
    virtual ~iI2C() = default;
    virtual II2CErrCode I2CWriteMem(uint16_t deviceAdd, uint16_t registerAdd, uint8_t txValue) = 0;
    virtual II2CErrCode I2CWriteMem(uint16_t deviceAdd,
                                    uint16_t registerAdd,
                                    uint8_t rxData[],
                                    uint16_t dataSize) = 0;
    virtual II2CErrCode I2CReadMem(uint16_t deviceAdd,
                                   uint16_t registerAdd,
                                   uint8_t rxData[],
                                   uint16_t dataSize) = 0;
    virtual II2CErrCode masterToSlaveTransmit(uint16_t add, uint8_t txData[], uint16_t dataSize) = 0;
    virtual II2CErrCode masterReceive(uint16_t add, uint8_t rxData[], uint16_t dataSize) = 0;
    //virtual II2CErrCode initI2C()=0;

protected:
    static const uint32_t gI2CTimeoutMs = 10000;

private:
};

#endif /* NEMEIO_I2C_II2C_II2C_H_ */
