#ifndef GTEST_HELPERS_HPP
#define GTEST_HELPERS_HPP

#include <cstdint>
#include <vector>
#include <cstring>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "utils.hpp"
#include "BleTxCmd.hpp"

#define EXPECT_ASSERTION_FAILED(statement) EXPECT_DEATH(statement, "Assertion.*failed")

void pushBackU32BE(std::vector<uint8_t>& vec, uint32_t val);
void pushBackU16BE(std::vector<uint8_t>& vec, uint16_t val);

ACTION_TEMPLATE(SetArgNPointeeToUInt16,
                HAS_1_TEMPLATE_PARAMS(unsigned, uIndex),
                AND_1_VALUE_PARAMS(value))
{
    std::memcpy(std::get<uIndex>(args), &value, sizeof(uint16_t));
}

ACTION_TEMPLATE(SetArgPointerContent,
                HAS_1_TEMPLATE_PARAMS(unsigned, uIndex),
                AND_2_VALUE_PARAMS(value, size))
{
    std::memcpy(std::get<uIndex>(args), value, size);
}

ACTION_TEMPLATE(SaveArgPointerContent,
                HAS_1_TEMPLATE_PARAMS(unsigned, uIndex),
                AND_2_VALUE_PARAMS(value, size))
{
    std::memcpy(value, std::get<uIndex>(args), size);
}

inline void pushBackU32BE(std::vector<uint8_t>& vec, uint32_t val)
{
    vec.push_back(static_cast<uint8_t>(val >> 24));
    vec.push_back(static_cast<uint8_t>(val >> 16));
    vec.push_back(static_cast<uint8_t>(val >> 8));
    vec.push_back(static_cast<uint8_t>(val));
}

inline void pushBackU16BE(std::vector<uint8_t>& vec, uint16_t val)
{
    vec.push_back(static_cast<uint8_t>(val >> 8));
    vec.push_back(static_cast<uint8_t>(val));
}

MATCHER_P(IsBleCmdVariant, id, "")
{
    const BleTxCmd& cmdBase = castToBase<BleTxCmd>(arg);
    return cmdBase.getId() == id;
}

#endif //GTEST_HELPERS_HPP
