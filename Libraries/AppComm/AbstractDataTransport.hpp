/*
 * AbstractDataTransport.hpp
 *
 *  Created on: Jan 31, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_ABSTRACTDATATRANSPORT_HPP_
#define APPCOMM_ABSTRACTDATATRANSPORT_HPP_

#include <cstddef>
#include <stdint.h>
#include "IDataTransportRxListener.hpp"

#include "Identifiable.hpp"

class AbstractDataTransport : public Identifiable
{
public:
    AbstractDataTransport(IDataTransportRxListener& rxListener);
    virtual ~AbstractDataTransport() = default;

    virtual void sendData(const uint8_t* pData, std::size_t len) = 0;

protected:
    void notifyReceivedData(const uint8_t* pData, std::size_t len);

private:
    IDataTransportRxListener& mRxListener;
};

inline AbstractDataTransport::AbstractDataTransport(IDataTransportRxListener& rxListener)
    : mRxListener(rxListener)
{
}

inline void AbstractDataTransport::notifyReceivedData(const uint8_t* pData, std::size_t len)
{
    mRxListener.dataReceived(*this, pData, len);
}

#endif /* APPCOMM_ABSTRACTDATATRANSPORT_HPP_ */
