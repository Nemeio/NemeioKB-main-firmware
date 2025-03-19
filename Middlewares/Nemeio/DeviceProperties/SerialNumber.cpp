/*
 * SerialNumber.cpp
 *
 *  Created on: 13 juil. 2021
 *      Author: thomas
 */

#include "SerialNumber.hpp"

SerialNumber::SerialNumber(etl::string_view serialNumber)
    : m_serialNumber(serialNumber)
{
}

etl::string_view SerialNumber::getString() const
{
    return m_serialNumber;
}

bool SerialNumber::isValid() const
{
    return m_serialNumber.full();
}

bool SerialNumber::operator==(const SerialNumber& serialNumber) const
{
    return serialNumber.m_serialNumber == m_serialNumber;
}

bool SerialNumber::operator!=(const SerialNumber& serialNumber) const
{
    return !(serialNumber == *this);
}