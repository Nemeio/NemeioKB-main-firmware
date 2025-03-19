/*
 * IDataTransportRxListener.hpp
 *
 *  Created on: Jan 31, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_IDATATRANSPORTRXLISTENER_HPP_
#define APPCOMM_IDATATRANSPORTRXLISTENER_HPP_

#include <cstdint>
#include <cstddef>
#include "../Connectivity/Identifiable.hpp"

class IDataTransportRxListener
{
public:
    IDataTransportRxListener() = default;
    virtual ~IDataTransportRxListener() = default;

    virtual void dataReceived(Identifiable& dataSource, const uint8_t* pData, std::size_t len) = 0;
};

#endif /* APPCOMM_IDATATRANSPORTRXLISTENER_HPP_ */
