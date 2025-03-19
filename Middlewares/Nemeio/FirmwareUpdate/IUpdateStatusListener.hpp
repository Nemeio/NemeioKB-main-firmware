/*
 * IUpdateStatusSender.hpp
 *
 *  Created on: 6 sept. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_IUPDATESTATUSLISTENER_HPP_
#define NEMEIO_FIRMWAREUPDATE_IUPDATESTATUSLISTENER_HPP_

#include "UpdateStatusDefs.hpp"

class IUpdateStatusListener
{
public:
    IUpdateStatusListener() = default;
    virtual ~IUpdateStatusListener() = default;

    virtual void notifyUpdateStatus(UpdateStatusDefs::UpdateStatus updateStatus) = 0;
};

#endif /* NEMEIO_FIRMWAREUPDATE_IUPDATESTATUSLISTENER_HPP_ */
