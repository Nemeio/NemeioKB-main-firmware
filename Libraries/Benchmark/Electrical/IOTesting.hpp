/*
 * IOTesting.hpp
 *
 *  Created on: 27 avr. 2021
 *      Author: thomas
 */

#ifndef BENCHMARK_ELECTRICAL_IOTESTING_HPP_
#define BENCHMARK_ELECTRICAL_IOTESTING_HPP_

#include "TestSuite.hpp"
#include "AssemblyHardware.h"
#include "ITestLog.hpp"
#include "embedded_ostream.hpp"

namespace benchmark
{
class IOTesting : public TestSuite
{
public:
    IOTesting(AssemblyHardware& hardwareAssembly, ITestLog& log);
    virtual ~IOTesting() = default;

    bool run() final;

    enum class IOTestingIndexes : uint8_t
    {
        SINGLE_IOTESTING_CHECK_ITEFLASH,
        SINGLE_IOTESTING_CHECK_EXTERNALFLASH,
        SINGLE_IOTESTING_CHECK_MATRIXIO,
    };

    bool runSingleTest(uint8_t ioTestingIndex);

private:
    /* Matrix */
    static constexpr GPIOPin m_keys[] = {
        Matrix_Row_One,
        Matrix_Row_Two,
        Matrix_Row_Three,
        Matrix_Row_Four,
        Matrix_Row_Five,
        Matrix_Row_Six,
        Matrix_Column_One,
        Matrix_Column_Two,
        Matrix_Column_Three,
        Matrix_Column_Four,
        Matrix_Column_Five,
        Matrix_Column_Six,
        Matrix_Column_Seven,
        Matrix_Column_Eight,
        Matrix_Column_Nine,
        Matrix_Column_Ten,
        Matrix_Column_Eleven,
        Matrix_Column_Twelve,
        Matrix_Column_Thirteen,
        Matrix_Column_Fourteen,
        Matrix_Column_Fifteen,
        Matrix_Column_Sixteen,
        Key75_Alt,
        Key73_Fn,
        Key74_Win,
        Key57_Enter,
        Key58_Shift_Left,
        Key71_Shift_Right,
        Key_Ctrl_Left,
        Key_Ctrl_Right,
        Key_Alt_Gr,
    };


    static constexpr uint8_t FRONTLIGHT_LED_MAX_INTENSITY = 100;
    static constexpr uint32_t DELAY_MATRIX_PIN_CHANGE_TAKES_EFFECT_MS = 5;
    static constexpr uint32_t FLASH_EXTERNAL_MANUFACTURER_INFO =
        0x00186001; /* Reserved, Device id LSB, Device id MSB, Manufacturer id */
    static constexpr uint32_t FLASH_ITE_MANUFACTURER_INFO =
        0x1620C2; /*Memory density, memory type, Manufacturer id */

    AssemblyHardware& m_hardwareAssembly;

    bool registerTestFromIndex(IOTestingIndexes ioTestingIndex);

    bool checkExternalFlash(embedded_ostream& description);
    bool checkITEFlash(embedded_ostream& description);
    bool checkMatrixIO(embedded_ostream& description);
};

} /* namespace benchmark */

#endif /* BENCHMARK_ELECTRICAL_IOTESTING_HPP_ */
