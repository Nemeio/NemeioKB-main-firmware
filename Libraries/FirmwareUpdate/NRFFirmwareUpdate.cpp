/*
 * NRFFirmwareUpdate.cpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */
#include <variant>
#include "DFUSelectObjectResponsePacket.hpp"
#include "DFUCRCResponsePacket.hpp"
#include "DFUCreateObjectPacket.hpp"
#include "DFUSelectObjectPacket.hpp"
#include "DFUSetReceipeNotificationPacket.hpp"
#include "DFUGetFirmwareVersionPacket.hpp"
#include "DFUFirmwareVersionResponsePacket.hpp"
#include "DFUPingPacket.hpp"
#include "DFUExecutePacket.hpp"
#include "DFUMTUPacket.hpp"
#include "DFUWriteObjectPacket.hpp"
#include "SLIPPacket.hpp"
#include "NRFFirmwareUpdate.hpp"
#include "AppCommDefs.hpp"
#include "FreeRTOS.h"
#include "FirmwareUpdateTimeouts.hpp"
#include "FreeRTOSUtils.hpp"

enum class NRFPackageState
{
    INIT,
    DATA,
};

NRFFirmwareUpdate::NRFFirmwareUpdate(IBleChip& bleChip)
    : mBleChip(bleChip)
    , mNRFFirmwarePackage(*this)
    , mDfuDataBuffer(std::bind(&NRFFirmwareUpdate::_bufferFull,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2))
{
    mDfuPacketQueue = xQueueCreate(MAX_NUMBER_OF_RECEIVED_SLIP_PACKET, sizeof(s_slipPacket));
}

NRFFirmwareUpdate::~NRFFirmwareUpdate()
{
    mBleChip.remove_observer(*this);
}

FirmwareUpdateDefs::ErrCode NRFFirmwareUpdate::initUpdate(uint32_t len)
{
    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;

    mBleChip.add_observer(*this);

    mNRFFirmwarePackage.init(len);
    mDfuDataBuffer.reset();

    mEnteredDfuSignal.clear();
    mBleChip.enterDfu();
    waitEnteredDfu();
    if(ping(fwupdatetimeouts::NRF_NB_PING_TO_CHECK_DFU_MODE))
    {
        ret = FirmwareUpdateDefs::ErrCode::SUCCESS;
    }
    else
    {
        // If entering DFU mode by the nRF application fails, force DFU mode and retry ping
        mEnteredDfuSignal.clear();
        mBleChip.forceDfu();
        waitEnteredDfu();
        if(ping(fwupdatetimeouts::NRF_NB_PING_TO_CHECK_DFU_MODE))
        {
            ret = FirmwareUpdateDefs::ErrCode::SUCCESS;
        }
    }

    return ret;
}

FirmwareUpdateDefs::ErrCode NRFFirmwareUpdate::writeFirmware(const uint8_t* pData, std::size_t len)
{
    return mNRFFirmwarePackage.receiveData(array_view<const uint8_t>(pData, len))
               ? FirmwareUpdateDefs::ErrCode::SUCCESS
               : FirmwareUpdateDefs::ErrCode::WRITE;
}

FirmwareUpdateDefs::ErrCode NRFFirmwareUpdate::runUpdate()
{
    /* https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.0.0%2Flib_bootloader_dfu_process.html
	 * NRF reboot itself at the end of an update */
    mBleChip.remove_observer(*this);
    return FirmwareUpdateDefs::ErrCode::SUCCESS;
}

void NRFFirmwareUpdate::notification(BleDfuResponse response)
{
    s_slipPacket packet;

    ASSERT(response.len <= sizeof(packet.data));

    packet.len = response.len;
    memcpy(packet.data, response.data, packet.len);

    xQueueSend(mDfuPacketQueue, &packet, 0);
}

void NRFFirmwareUpdate::notification(BleEvent event)
{
    if(event == BleEvent::ENTERED_DFU)
    {
        mEnteredDfuSignal.set();
    }
}

uint32_t NRFFirmwareUpdate::getVersion(NRFImageNumber image)
{
    DFUGetFirmwareVersionPacket firmwareVersionPacket(static_cast<uint8_t>(image));

    uint32_t version = 0;
    uint8_t nbTimeout = 0;

    do
    {
        if(DFUFirmwareVersionResponsePacket responsePacket;
           sendAndWaitForReply(firmwareVersionPacket,
                               fwupdatetimeouts::NRF_DFU_PACKET_TIMEOUT_MS,
                               responsePacket,
                               firmwareVersionPacket.getCommand())
           && responsePacket.getResponseCode() == DFUResponseCode::SUCCESS)
        {
            version = responsePacket.getVersion();
        }
        else
        {
            ++nbTimeout;
        }
    } while(!version && nbTimeout < 3);

    return version;
}

bool NRFFirmwareUpdate::waitEnteredDfu()
{
    return mEnteredDfuSignal.wait(DelayMs(fwupdatetimeouts::NRF_WAIT_ENTERED_DFU_TIMEOUT_MS));
}

bool NRFFirmwareUpdate::ping(uint32_t nbRetry)
{
    uint8_t nbTimeout = 0;
    uint32_t pingId = 0;
    bool pingReceived = false;

    do
    {
        DFUPingPacket ping(++pingId);
        if(DFUResponsePacket responsePacket;
           sendAndWaitForReply(ping,
                               fwupdatetimeouts::NRF_DFU_PACKET_TIMEOUT_MS,
                               responsePacket,
                               ping.getCommand())
           && responsePacket.payloadSize() > 0
           && (*reinterpret_cast<const uint32_t*>(responsePacket.payloadData())) == pingId)
        {
            pingReceived = true;
        }
        else
        {
            ++nbTimeout;
        }
    } while(!pingReceived && nbTimeout < nbRetry);

    return pingReceived;
}

bool NRFFirmwareUpdate::setPrn()
{
    bool prnReceived = false;
    DFUSetReceipeNotificationPacket prn(1); /* NRF must reply to each Write Command */

    if(DFUResponsePacket responsePacket;
       sendAndWaitForReply(prn,
                           fwupdatetimeouts::NRF_DFU_PACKET_TIMEOUT_MS,
                           responsePacket,
                           prn.getCommand())
       && responsePacket.getResponseCode() == DFUResponseCode::SUCCESS)
    {
        prnReceived = true;
    }

    return prnReceived;
}

uint16_t NRFFirmwareUpdate::getMtu()
{
    uint16_t mtuValue = 0;
    DFUMTUPacket mtu;

    if(DFUResponsePacket responsePacket;
       sendAndWaitForReply(mtu,
                           fwupdatetimeouts::NRF_DFU_PACKET_TIMEOUT_MS,
                           responsePacket,
                           mtu.getCommand())
       && responsePacket.getResponseCode() == DFUResponseCode::SUCCESS
       && responsePacket.payloadSize() == sizeof(mtuValue))
    {
        mtuValue = *(reinterpret_cast<const uint16_t*>(responsePacket.payloadData()));
    }

    return mtuValue;
}

NRFExecuteStatus NRFFirmwareUpdate::execute()
{
    NRFExecuteStatus status = NRFExecuteStatus::UNKNOWN;
    DFUExecutePacket execute{};

    if(DFUResponsePacket responsePacket;
       sendAndWaitForReply(execute,
                           fwupdatetimeouts::NRF_DFU_PACKET_TIMEOUT_MS,
                           responsePacket,
                           execute.getCommand()))
    {
        if(responsePacket.getResponseCode() == DFUResponseCode::SUCCESS)
        {
            status = NRFExecuteStatus::SUCCESS;
        }
        else if(responsePacket.getResponseCode() == DFUResponseCode::EXTENDED_ERROR
                && (responsePacket.getExtendedResponseCode()
                    == DFUExtendedResponseCode::FW_VERSION_FAILURE))
        {
            status = NRFExecuteStatus::IGNORE;
        }
    }

    return status;
}

static DFUObjectType packageTypeToDFUObject(NRFFirmwarePackageType type)
{
    DFUObjectType dfuType = DFUObjectType::FIRMWARE_INVALID;
    switch(type)
    {
    case NRFFirmwarePackageType::FIRMWARE_INIT:
        dfuType = DFUObjectType::FIRMWARE_INIT;
        break;
    case NRFFirmwarePackageType::FIRMWARE_DATA:
        dfuType = DFUObjectType::FIRMWARE_DATA;
        break;
    default:
        dfuType = DFUObjectType::FIRMWARE_INVALID;
        break;
    }

    return dfuType;
}

bool NRFFirmwareUpdate::onManifest(NRFFirmwarePackageType type, size_t len)
{
    DFUSelectObjectResponsePacket responsePacket;
    bool ret = false;

    mCurrentFirmwareType = type;
    mCurrentObjectOffset = 0;
    mCurrentObjectSize = 0;
    mCurrentFirmwareSize = 0;
    mRemainingFirmwareSize = 0;
    mDfuDataBuffer.reset();

    if(mIgnoreNextFirmware && type == NRFFirmwarePackageType::FIRMWARE_DATA)
    {
        return true;
    }

    if(type == NRFFirmwarePackageType::FIRMWARE_INIT)
    {
        mIgnoreNextFirmware = false;

        mEnteredDfuSignal.clear();

        // Check if we are already in dfu mode to avoid waiting for entered dfu event if not necessary
        bool pingSuccess = ping(fwupdatetimeouts::NRF_NB_PING_TO_CHECK_DFU_MODE);

        if(!pingSuccess)
        {
            waitEnteredDfu();
            pingSuccess = ping(fwupdatetimeouts::NRF_NB_PING_TO_CHECK_DFU_MODE);
        }

        if(pingSuccess)
        {
            uint16_t mtu = getMtu();
            if(mtu > 0 && mtu > mDfuDataBuffer.size() && setPrn())
            {
                ret = true;
            }
        }
    }
    else
    {
        ret = true;
    }

    if(ret)
    {
        /* Send Select packet */
        DFUSelectObjectPacket selectObject(packageTypeToDFUObject(mCurrentFirmwareType));
        ret = sendAndWaitForReply(selectObject,
                                  fwupdatetimeouts::NRF_DFU_PACKET_TIMEOUT_MS,
                                  responsePacket,
                                  selectObject.getCommand())
              && (responsePacket.getResponseCode() == DFUResponseCode::SUCCESS);
    }

    if(ret)
    {
        mCurrentObjectSize = responsePacket.getMaxSize();
        ret = (mCurrentObjectSize % mDfuDataBuffer.size() == 0);
    }

    if(ret)
    {
        mRemainingFirmwareSize = mCurrentFirmwareSize = len;
    }

    return ret;
}

bool NRFFirmwareUpdate::onFirmware(array_view<const uint8_t> data)
{
    if(mIgnoreNextFirmware)
    {
        return true;
    }

    return mDfuDataBuffer.pushBack(data.data(), data.data() + data.size()) == data.size();
}

bool NRFFirmwareUpdate::onFirmwareEnd()
{
    return mDfuDataBuffer.partialFlush();
}

bool NRFFirmwareUpdate::send(BleDfuDataCmd& cmd, IDFUPacket& packet)
{
    SLIPPacket<SLIP_DATA_MAX_SIZE> slipPacket;
    slipPacket.createWithPayload(array_view<const uint8_t>(packet.data(), packet.size()));
    DFUResponsePacket dfuResponsPacket;

    xQueueReset(mDfuPacketQueue);

    cmd.setData(slipPacket.data(), slipPacket.size());

    return (mBleChip.sendCommand(BleDfuModeTxCommandRef(etl::reference_wrapper(cmd)))
            == BLEDefs::ErrCode::SUCCESS);
}

bool NRFFirmwareUpdate::receive(DFUResponsePacket& packet, uint32_t timeoutMs)
{
    bool response = false;
    s_slipPacket dataReceived = {};
    ssize_t slipPacketSize;

    if(pdTRUE
       == xQueueReceive(mDfuPacketQueue, &dataReceived, delayToSystemTicks(DelayMs(timeoutMs))))
    {
        SLIPPacket<SLIP_DATA_MAX_SIZE> slipPacket(
            array_view<const uint8_t>(dataReceived.data, dataReceived.len));
        mResponsePayload.fill(0);
        slipPacketSize = slipPacket.getPayload(
            array_view<uint8_t>(mResponsePayload.data(), mResponsePayload.size()));

        if(packet.setData(array_view<const uint8_t>(mResponsePayload.data(), slipPacketSize)) > 0
           && packet.isValidResponse())
        {
            response = true;
        }
    }

    return response;
}

bool NRFFirmwareUpdate::sendAndWaitForReply(IDFUPacket& packet,
                                            uint32_t timeout,
                                            DFUResponsePacket& response,
                                            DFUCommand responseCommand)
{
    bool ret = false;
    uint8_t nbRetry = fwupdatetimeouts::NRF_NB_DFU_PACKET_MAX_TIMEOUT;

    do
    {
        BleDfuDataCmd dfuCmd;
        mBleChip.registerBleCommandListener(dfuCmd);

        dfuCmd.add_observer(*this);
        if(send(dfuCmd, packet) && receive(response, timeout)
           && response.getCommand() == responseCommand)
        {
            ret = true;
        }
        nbRetry--;

        mBleChip.unregisterBleCommandListener(dfuCmd);
    } while(!ret && nbRetry > 0);

    return ret;
}

size_t NRFFirmwareUpdate::_bufferFull(const uint8_t* data, size_t size)
{
    bool ret = false;
    DFUResponsePacket responsePacket;

    ASSERT((mCurrentObjectOffset + size) <= mCurrentObjectSize);

    if(mCurrentObjectOffset == 0)
    {
        DFUCreateObjectPacket createObject(packageTypeToDFUObject(mCurrentFirmwareType),
                                           std::min(mRemainingFirmwareSize, mCurrentObjectSize));
        ret = sendAndWaitForReply(createObject,
                                  fwupdatetimeouts::NRF_DFU_PACKET_TIMEOUT_MS,
                                  responsePacket,
                                  createObject.getCommand())
              && (responsePacket.getResponseCode() == DFUResponseCode::SUCCESS);
        mCurrentObjectOffset = 0;
    }
    else
    {
        ret = true;
    }

    if(ret)
    {
        DFUCRCResponsePacket crcResponsePacket;
        DFUWriteObjectPacket<DFU_PACKET_MAX_SIZE> writeObject(array_view<const uint8_t>(data, size));
        mRemainingFirmwareSize -= size;
        mCurrentObjectOffset += size;
        ret = sendAndWaitForReply(writeObject,
                                  fwupdatetimeouts::NRF_DFU_PACKET_TIMEOUT_MS,
                                  crcResponsePacket,
                                  DFUCommand::CALCULATE_CHECKSUM)
              && (crcResponsePacket.getResponseCode() == DFUResponseCode::SUCCESS)
              && crcResponsePacket.getOffset() == (mCurrentFirmwareSize - mRemainingFirmwareSize);
    }

    if(ret && (mCurrentObjectOffset == mCurrentObjectSize || mRemainingFirmwareSize == 0))
    {
        NRFExecuteStatus execStatus = NRFExecuteStatus::UNKNOWN;
        mCurrentObjectOffset = 0;
        execStatus = execute();

        if(execStatus == NRFExecuteStatus::SUCCESS)
        {
            ret = true;
        }
        else if(execStatus == NRFExecuteStatus::IGNORE)
        {
            mIgnoreNextFirmware = true;
            ret = true;
        }
        else
        {
            ret = false;
        }
    }

    return ret ? size : 0;
}
