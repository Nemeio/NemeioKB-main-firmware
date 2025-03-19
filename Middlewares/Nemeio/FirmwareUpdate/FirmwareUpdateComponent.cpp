/*
 * FirmwareUpdateComponent.cpp
 *
 *  Created on: 17 févr. 2021
 *      Author: thomas
 */

#include "FirmwareUpdateComponent.hpp"

FirmwareUpdateComponent::FirmwareUpdateComponent(Identification::ModuleId componentId,
                                                 IFirmwareUpdate& updater)
    : mModuleId(componentId)
    , mUpdater(updater)
{
}

IFirmwareUpdate& FirmwareUpdateComponent::getUpdater()
{
    return mUpdater;
}

Identification::ModuleId FirmwareUpdateComponent::getModuleId()
{
    return mModuleId;
}
