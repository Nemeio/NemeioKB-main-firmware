/*
 * AbstractBleCommandListener.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: Labo
 */

#include "AbstractBleCommandListener.hpp"

AbstractBleCommandListener::AbstractBleCommandListener(BLESPICommands::CommandId commandID)
    : mCommandID(commandID)
{
}

BLESPICommands::CommandId AbstractBleCommandListener::getCommandId() const
{
    return mCommandID;
}