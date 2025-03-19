/*
 * IAppCommCmd.hpp
 *
 *  Created on: 20 août 2021
 *      Author: thomas
 */

#ifndef NEMEIO_APPCOMM_IAPPCOMMCMD_HPP_
#define NEMEIO_APPCOMM_IAPPCOMMCMD_HPP_

#include "AppCommDefs.hpp"

class IAppCommCmd
{
public:
    virtual AppCommDefs::AppCommCmd getCmdId() const = 0;
    virtual void dataReceived(const uint8_t* pData, size_t dataLen) = 0;
};


#endif /* NEMEIO_APPCOMM_IAPPCOMMCMD_HPP_ */
