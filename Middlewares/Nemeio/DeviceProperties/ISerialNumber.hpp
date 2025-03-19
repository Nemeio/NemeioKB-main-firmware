/*
 * ISerialNumber.hpp
 *
 *  Created on: 26 juil. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DEVICEPROPERTIES_ISERIALNUMBER_HPP_
#define NEMEIO_DEVICEPROPERTIES_ISERIALNUMBER_HPP_

#include <cstddef>
#include "etl/string_view.h"

static constexpr size_t SERIAL_NUMBER_MAX_SIZE = 12;

class ISerialNumber
{
public:
    ISerialNumber() = default;
    virtual ~ISerialNumber() = default;

    virtual etl::string_view getString() const = 0;
};


#endif /* NEMEIO_DEVICEPROPERTIES_ISERIALNUMBER_HPP_ */
