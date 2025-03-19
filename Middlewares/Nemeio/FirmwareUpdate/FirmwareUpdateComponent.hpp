/*
 * FirmwareUpdateComponent.hpp
 *
 *  Created on: 17 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATECOMPONENT_HPP_
#define NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATECOMPONENT_HPP_

#include <cstdint>
#include "IUpdateListener.hpp"
#include "IFirmwareUpdate.hpp"
#include "Identification.hpp"
#include "IUpdateStatusListener.hpp"

class FirmwareUpdateComponent
{
public:
    enum FirmwareUpdateError : uint8_t
    {
        UNKNOWN = 0,
    };

    FirmwareUpdateComponent(Identification::ModuleId componentId, IFirmwareUpdate& updater);
    [[nodiscard]] IFirmwareUpdate& getUpdater();
    [[nodiscard]] Identification::ModuleId getModuleId();

private:
    Identification::ModuleId mModuleId;
    IFirmwareUpdate& mUpdater;
};

#endif /* NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATECOMPONENT_HPP_ */
