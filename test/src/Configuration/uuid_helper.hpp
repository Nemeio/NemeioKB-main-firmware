#pragma once

#include "ConfigurationStorageUtil.hpp"

uuid::Uuid makeUuid(std::string str)
{
    auto optionalUuid = ConfigurationStorageUtil::strToId(str.c_str());
    return optionalUuid.value_or(uuid::Uuid(etl::array<std::byte, uuid::SIZE>{std::byte(0x00)}));
}