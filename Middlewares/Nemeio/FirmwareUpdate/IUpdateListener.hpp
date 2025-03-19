/*
 * IUpgradeListener.hpp
 *
 *  Created on: 11 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_IUPDATELISTENER_HPP_
#define NEMEIO_FIRMWAREUPDATE_IUPDATELISTENER_HPP_

#include <cstdint>

enum class UpdateStatus
{
    START,
    END,
    ERROR,
};

class IUpdateListener
{
public:
    virtual ~IUpdateListener() = default;

    virtual void onUpdateEvent(UpdateStatus status) = 0;
};

#endif /* NEMEIO_FIRMWAREUPDATE_IUPDATELISTENER_HPP_ */
