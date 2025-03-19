/*
 * IDataBytes.hpp
 *
 *  Created on: Jul 3, 2019
 *      Author: Labo
 */

#ifndef IDATABYTES_HPP_
#define IDATABYTES_HPP_

#include <cstdint>
#include <iterator>

class IDataBytes
{
public:
    IDataBytes() = default;
    virtual ~IDataBytes() = default;

    virtual void init() = 0;
    virtual bool get(uint8_t* pBuff, std::size_t buffSize, std::size_t& retSize) const = 0;
    virtual bool getString(std::string& serialNumberStr) = 0;
};

#endif /* IDATABYTES_HPP_ */
