/*
 * MacAddress.hpp
 *
 *  Created on: 13 juil. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DEVICEPROPERTIES_MACADDRESS_HPP_
#define NEMEIO_DEVICEPROPERTIES_MACADDRESS_HPP_

#include <cstdint>
#include <cstddef>
#include "etl/array_view.h"
#include "embedded_ostream.hpp"

static constexpr size_t MAC_ADDRESS_SIZE_BYTES = 6;

class MacAddress
{
public:
    static constexpr size_t SIZE = MAC_ADDRESS_SIZE_BYTES;

    MacAddress() = delete;
    explicit MacAddress(etl::array<uint8_t, SIZE> mac);
    virtual ~MacAddress() = default;

    etl::array_view<const uint8_t> get() const { return m_macAddress; }

    bool isValid() const;

    bool operator==(const MacAddress& macAddress) const;
    bool operator!=(const MacAddress& macAddress) const;

private:
    etl::array<uint8_t, SIZE> m_macAddress;

    friend embedded_ostream& operator<<(embedded_ostream& os, const MacAddress& mac);
};

#endif /* NEMEIO_DEVICEPROPERTIES_MACADDRESS_HPP_ */
