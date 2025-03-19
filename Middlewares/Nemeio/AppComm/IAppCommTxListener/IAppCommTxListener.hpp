/*
 * IAppCommTxListener.hpp
 *
 *  Created on: Feb 6, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_APPCOMM_IAPPCOMMTXLISTENER_IAPPCOMMTXLISTENER_HPP_
#define NEMEIO_APPCOMM_IAPPCOMMTXLISTENER_IAPPCOMMTXLISTENER_HPP_

#include <cstddef>
#include "AppCommDefs.hpp"

class IAppCommTxListener
{
public:
    IAppCommTxListener() = default;
    virtual ~IAppCommTxListener() = default;

    virtual void sendCmd(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len) = 0;
    virtual void sendResponse(AppCommDefs::AppCommCmd cmd,
                              const uint8_t* pData,
                              std::size_t len) = 0;
};

#endif /* NEMEIO_APPCOMM_IAPPCOMMTXLISTENER_IAPPCOMMTXLISTENER_HPP_ */
