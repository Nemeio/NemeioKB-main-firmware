/*
 * IOTesting.cpp
 *
 *  Created on: 27 avr. 2021
 *      Author: thomas
 */

#include "IOTesting.hpp"
#include "utils.hpp"
#include <functional>
#include <array>
#include <utility>
#include <cstdint>

namespace benchmark
{
IOTesting::IOTesting(AssemblyHardware& hardwareAssembly, ITestLog& log)
    : TestSuite(log)
    , m_hardwareAssembly(hardwareAssembly)
{
}


bool IOTesting::run()
{
    bool ret = true;

    /* Read ITE flash ID must be done first, because you can't do it while ITE is running */
    ret = ret && registerTestFromIndex(IOTestingIndexes::SINGLE_IOTESTING_CHECK_ITEFLASH);
    ret = ret && registerTestFromIndex(IOTestingIndexes::SINGLE_IOTESTING_CHECK_EXTERNALFLASH);
    ret = ret && registerTestFromIndex(IOTestingIndexes::SINGLE_IOTESTING_CHECK_MATRIXIO);

    if(ret)
    {
        ret = runTests("IO Testing");
    }

    unregisterAllTests();

    return ret;
}

bool IOTesting::runSingleTest(uint8_t ioTestingIndex)
{
    bool ret = registerTestFromIndex(static_cast<IOTestingIndexes>(ioTestingIndex));

    if(ret)
    {
        ret = runTests("Single IO Test");
    }

    unregisterAllTests();

    return ret;
}

bool IOTesting::registerTestFromIndex(IOTestingIndexes ioTestingIndex)
{
    bool ret = true;

    switch(ioTestingIndex)
    {
    case IOTesting::IOTestingIndexes::SINGLE_IOTESTING_CHECK_ITEFLASH:
        registerTest("ITE Flash", std::bind(&IOTesting::checkITEFlash, this, std::placeholders::_1));
        break;
    case IOTesting::IOTestingIndexes::SINGLE_IOTESTING_CHECK_EXTERNALFLASH:
        registerTest("External Flash",
                     std::bind(&IOTesting::checkExternalFlash, this, std::placeholders::_1));
        break;
    case IOTesting::IOTestingIndexes::SINGLE_IOTESTING_CHECK_MATRIXIO:
        registerTest("Matrix", std::bind(&IOTesting::checkMatrixIO, this, std::placeholders::_1));
        break;
    default:
        ret = false;
    }

    return ret;
}

bool IOTesting::checkExternalFlash(embedded_ostream& description)
{
    bool ret = true;

    m_hardwareAssembly.getWatchdogItem().refresh();

    /* Get flash intformation */
    uint32_t manufacturerInfo = m_hardwareAssembly.getExternalFlash().getIdentifier();
    if(manufacturerInfo != FlashFactory::FLASH_EXTERNAL_CYPRESS_32MB_MANUFACTURER_INFO
       && manufacturerInfo != FlashFactory::FLASH_EXTERNAL_MACRONIX_32MB_MANUFACTURER_INFO)
    {
        ret = false;
    }

    uint64_t uniqueId = m_hardwareAssembly.getExternalFlash().getUniqueId();
    if(uniqueId == 0)
    {
        ret = false;
    }

    if(!ret)
    {
        description << "Manufacturer info is " << manufacturerInfo << " (expected value was "
                    << FLASH_EXTERNAL_MANUFACTURER_INFO << ") "
                    << " and unique id is " << uniqueId;
    }

    return ret;
}

bool IOTesting::checkITEFlash(embedded_ostream& description)
{
    bool ret = true;

    m_hardwareAssembly.getWatchdogItem().refresh();
    m_hardwareAssembly.initITEFlash();

    /* Get flash information */
    uint32_t info;
    info = m_hardwareAssembly.getITEFlash().getIdentifier();

    if(info != FLASH_ITE_MANUFACTURER_INFO || info == 0)
    {
        ret = false;
    }

    if(!ret)
    {
        description << "Manufacturer info is " << info << " (expected value was "
                    << FLASH_ITE_MANUFACTURER_INFO << ") ";
    }

    m_hardwareAssembly.deinitITEFlash();

    return ret;
}


bool IOTesting::checkMatrixIO(embedded_ostream& description)
{
    bool ret = true;
    std::vector<std::pair<uint8_t, uint8_t>> errors;
    IGPIOController& gpioController = m_hardwareAssembly.getStm32GPIOController();

    const GPIOOutConfig rowGpioOutputConfig = {
        .outType = IGPIO_OUTTYPE_OPENDRAIN,
        .pull = IGPIO_PULL_UP,
    };

    /* Configure as output */
    for(auto key: m_keys)
    {
        gpioController.configureOutput(key, rowGpioOutputConfig);
        gpioController.set(key);

        m_hardwareAssembly.getSystemItem().delay(DELAY_MATRIX_PIN_CHANGE_TAKES_EFFECT_MS);

        if(gpioController.read(key) != IGPIO_STATE_HIGH)
        {
            errors.push_back(std::make_pair(key, key));
            ret = false;
        }
    }

    /* Change output and verify that columns has the expected state */
    for(auto key: m_keys)
    {
        gpioController.clear(key);
        m_hardwareAssembly.getSystemItem().delay(DELAY_MATRIX_PIN_CHANGE_TAKES_EFFECT_MS);

        /* Check short circuit */
        for(auto internal_key: m_keys)
        {
            if((internal_key == key && gpioController.read(internal_key) != IGPIO_STATE_LOW)
               || /* Current changed row */
               (internal_key != key && gpioController.read(internal_key) != IGPIO_STATE_HIGH))
            { /* Other rows */
                errors.push_back(std::make_pair(internal_key, key));
                ret = false;
            }
        }

        gpioController.set(key);
        m_hardwareAssembly.getSystemItem().delay(DELAY_MATRIX_PIN_CHANGE_TAKES_EFFECT_MS);
    }

    if(!ret)
    {
        description << "Short circuit between the following pins: ";
        for(auto& value: errors)
        {
            description << "(" << value.first << "," << value.second << ") ";
        }
    }

    return ret;
}

} /* namespace benchmark */
