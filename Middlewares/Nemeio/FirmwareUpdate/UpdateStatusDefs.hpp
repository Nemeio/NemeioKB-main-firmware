/*
 * UpdateStatusDefs.hpp
 *
 *  Created on: 7 sept. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_UPDATESTATUSDEFS_HPP_
#define NEMEIO_FIRMWAREUPDATE_UPDATESTATUSDEFS_HPP_

#include <cstdint>
#include "Identification.hpp"

namespace UpdateStatusDefs
{
enum class UpdateStatusNotificationType
{
    UNKNOWN = 0,
    UPDATE_FAILED = 1,
    UPDATE_PROGRESS = 2,
    ROLLBACK_PROGRESS = 3,
};

struct UpdateStatus
{
    Identification::ModuleId mComponent;
    UpdateStatusNotificationType mType;
    uint8_t mPayload;

    UpdateStatus(Identification::ModuleId component,
                 UpdateStatusNotificationType type,
                 uint8_t payload)
        : mComponent(component)
        , mType(type)
        , mPayload(payload){};
};
}; // namespace UpdateStatusDefs


#endif /* NEMEIO_FIRMWAREUPDATE_UPDATESTATUSDEFS_HPP_ */
