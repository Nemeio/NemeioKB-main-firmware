/*
 * IFlash.hpp
 *
 *  Created on: 2 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_IFLASH_HPP_
#define SFLASH_IFLASH_HPP_

#include "IFlashIO.hpp"
#include "IFlashConfiguration.hpp"

class IFlash : public IFlashIO, public IFlashConfiguration
{
public:
    virtual bool init() = 0;
    virtual bool deinit() = 0;
};


#endif /* SFLASH_IFLASH_HPP_ */
