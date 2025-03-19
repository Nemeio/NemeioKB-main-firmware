/*
 * AbstractSerialAdapter.hpp
 *
 *  Created on: Jun 5, 2019
 *      Author: Labo
 */

#ifndef LOG_ABSTRACTSERIALADAPTER_HPP_
#define LOG_ABSTRACTSERIALADAPTER_HPP_

#include <array>
#include <cstdint>
#include "etl/type_traits.h"
#include "etl/string_view.h"

class AbstractSerialAdapter
{
public:
    AbstractSerialAdapter() = default;
    virtual ~AbstractSerialAdapter() = default;

    // Implementation for integral types
    template<typename T, typename etl::enable_if_t<etl::is_integral_v<T>, int>* = nullptr>
    AbstractSerialAdapter& operator<<(const T& data);

    AbstractSerialAdapter& operator<<(etl::string_view data);

private:
    virtual std::size_t writeBuffer(const uint8_t* pData, std::size_t len) = 0;
};

template<typename T, typename etl::enable_if_t<etl::is_integral_v<T>, int>*>
AbstractSerialAdapter& AbstractSerialAdapter::operator<<(const T& data)
{
    writeBuffer(reinterpret_cast<const uint8_t*>(&data), sizeof(data));

    return (*this);
}

#endif /* LOG_ABSTRACTSERIALADAPTER_HPP_ */
