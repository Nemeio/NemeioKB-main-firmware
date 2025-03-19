/*
 * SerialNumberCmd.hpp
 *
 *  Created on: Jul 3, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_SERIALNUMBER_SERIALNUMBERCMD_HPP_
#define APPCOMM_SERIALNUMBER_SERIALNUMBERCMD_HPP_

#include "SerialNumber.hpp"
#include "AppCommCmd.hpp"
#include <cstring>

class SerialNumberCmd : public AppCommCmd
{
public:
    SerialNumberCmd(IAppCommTxListener& appCommTxListener, const ISerialNumber& serialNumber);
    virtual ~SerialNumberCmd() = default;

private:
    void dataReceived(const uint8_t* pData, size_t dataLen);

    const ISerialNumber& mSerialNumber;
};

#endif /* APPCOMM_SERIALNUMBER_SERIALNUMBERCMD_HPP_ */
