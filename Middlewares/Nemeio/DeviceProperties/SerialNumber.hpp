/*
 * SerialNumber.hpp
 *
 *  Created on: 13 juil. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DEVICEPROPERTIES_SERIALNUMBER_HPP_
#define NEMEIO_DEVICEPROPERTIES_SERIALNUMBER_HPP_

#include <cstdint>
#include <cstddef>
#include "etl/string.h"
#include "ISerialNumber.hpp"

class SerialNumber : public ISerialNumber
{
public:
    static constexpr size_t SIZE = 12;

    SerialNumber() = delete;
    explicit SerialNumber(etl::string_view serialNumber);
    virtual ~SerialNumber() final = default;

    etl::string_view getString() const final;

    bool isValid() const;

    bool operator==(const SerialNumber& serialNumber) const;
    bool operator!=(const SerialNumber& serialNumber) const;

private:
    etl::string<SIZE> m_serialNumber;
};

#endif /* NEMEIO_DEVICEPROPERTIES_SERIALNUMBER_HPP_ */
