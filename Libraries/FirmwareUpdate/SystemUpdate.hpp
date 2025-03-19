/*
 * SystemUpdate.hpp
 *
 *  Created on: 21 sept. 2021
 *      Author: thomas
 */

#ifndef FIRMWAREUPDATE_SYSTEMUPDATE_HPP_
#define FIRMWAREUPDATE_SYSTEMUPDATE_HPP_

#include <optional>
#include "UpdateEngine.hpp"
#include "IFirmwareUpdate.hpp"

class SystemUpdate : public IFirmwareUpdate, public IUpdateListener, public IUpdateStatusListener
{
public:
    explicit SystemUpdate(UpdateEngine& engine, ISystemUpdateFile& file);
    virtual ~SystemUpdate() = default;

    FirmwareUpdateDefs::ErrCode initUpdate(uint32_t len) final;
    FirmwareUpdateDefs::ErrCode writeFirmware(const uint8_t* pData, std::size_t len) final;
    FirmwareUpdateDefs::ErrCode runUpdate() final;

    bool registerListener(IUpdateListener& listener);
    bool registerComponentUpdateListener(IUpdateStatusListener& listener);


private:
    UpdateEngine& mUpdateEngine;
    ISystemUpdateFile& mSystemUpdateFile;

    std::vector<std::reference_wrapper<IUpdateListener>> mUpdateListener;
    std::vector<std::reference_wrapper<IUpdateStatusListener>> mUpdateStatusListener;


    void onUpdateEvent(UpdateStatus status) final;
    void notifyUpdateStatus(UpdateStatusDefs::UpdateStatus updateStatus) final;
};

#endif /* FIRMWAREUPDATE_SYSTEMUPDATE_HPP_ */
