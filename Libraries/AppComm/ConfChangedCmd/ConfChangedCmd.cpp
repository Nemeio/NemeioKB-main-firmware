/*
 * ConfChangedCmd.cpp
 *
 *  Created on: Feb 28, 2019
 *      Author: Labo
 */

#include "ConfChangedCmd.hpp"
#include "assertTools.h"

ConfChangedCmd::ConfChangedCmd(IAppCommTxListener& appCommTxListener)
    : AppCommCmd(AppCommDefs::AppCommCmd::LAYOUTCONFIGCHANGED, appCommTxListener)
{
}

void ConfChangedCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
}

void ConfChangedCmd::configurationChanged(const Configuration* pConfig)
{
    ASSERT(pConfig != nullptr);
    uuid::UuidView layoutConfigurationId = pConfig->getId();

    transmitCmd(reinterpret_cast<const uint8_t*>(layoutConfigurationId.get().data()),
                layoutConfigurationId.get().size());
}
