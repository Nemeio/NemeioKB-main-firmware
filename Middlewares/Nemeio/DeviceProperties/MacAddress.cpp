/*
 * MacAddress.cpp
 *
 *  Created on: 13 juil. 2021
 *      Author: thomas
 */

#include <MacAddress.hpp>
#include "etl/algorithm.h"

MacAddress::MacAddress(etl::array<uint8_t, SIZE> mac)
{
    etl::copy(mac.begin(), mac.end(), m_macAddress.begin());
}

bool MacAddress::isValid() const
{
    return m_macAddress != etl::array<uint8_t, MacAddress::SIZE>{0};
}

bool MacAddress::operator==(const MacAddress& macAddress) const
{
    return macAddress.m_macAddress == m_macAddress;
}

bool MacAddress::operator!=(const MacAddress& macAddress) const
{
    return !(macAddress == *this);
}

embedded_ostream& operator<<(embedded_ostream& os, const MacAddress& mac)
{
    os << etl::hex;

    bool first = true;
    for(auto byte: mac.m_macAddress)
    {
        if(!first)
        {
            os << ":";
        }

        os << etl::setw(2) << etl::setfill('0') << byte << etl::setw(0) << etl::setfill(' ');
        first = false;
    }

    os << etl::dec;

    return os;
}