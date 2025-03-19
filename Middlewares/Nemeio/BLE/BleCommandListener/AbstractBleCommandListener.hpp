/*
 * AbstractBleCommandListener.hpp
 *
 *  Created on: Oct 22, 2018
 *      Author: Labo
 */

#ifndef BLE_ABSTRACTBLECHIPCOMMANDLISTENER_HPP_
#define BLE_ABSTRACTBLECHIPCOMMANDLISTENER_HPP_

#include <cstddef>
#include <stdint.h>
#include "BLESPICommands.hpp"
#include "etl/array_view.h"

class AbstractBleCommandListener
{
public:
    explicit AbstractBleCommandListener(BLESPICommands::CommandId commandID);
    virtual ~AbstractBleCommandListener() = default;

    virtual void receivedData(const uint8_t* data, size_t len) = 0;
    BLESPICommands::CommandId getCommandId() const;

private:
    BLESPICommands::CommandId mCommandID;
};

#endif /* BLE_ABSTRACTBLECHIPCOMMANDLISTENER_HPP_ */
