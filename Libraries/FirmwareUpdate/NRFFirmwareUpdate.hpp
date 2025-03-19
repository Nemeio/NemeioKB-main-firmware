/*
 * NRFFirmwareUpdate.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_UPDATER_NRFFIRMWAREUPDATE_HPP_
#define NEMEIO_FIRMWAREUPDATE_UPDATER_NRFFIRMWAREUPDATE_HPP_

#include <array>
#include "AbstractBleCommandListener.hpp"
#include "IBleChip.hpp"
#include "DFUPacket.hpp"
#include "DFUResponsePacket.hpp"
#include "IFirmwareUpdate.hpp"
#include "NRFFirmwarePackage.hpp"
#include "INRFFirmwarePackageListener.hpp"
#include "FlashWriteBuffer.hpp"
#include "BleDfuDataCmd.hpp"
#include "BleEventObserver.hpp"
#include "Signal.hpp"
#include "cpp_helper.hpp"

enum class NRFExecuteStatus
{
    UNKNOWN,
    SUCCESS,
    IGNORE,
};

/* NRF image number and NRF image type are not matching */
enum class NRFImageNumber
{
    NRF_BOOTLOADER = 0,
    NRF_SOFT_DEVICE = 1,
    NRF_APPLICATION = 2,
    NRF_UNKNOWN = 0xFF
};

class NRFFirmwareUpdate : public IFirmwareUpdate,
                          public BleDfuDataCmdObserver,
                          public BleEventObserver,
                          private INRFFirmwarePackageListener
{
public:
    explicit NRFFirmwareUpdate(IBleChip& bleChip);
    virtual ~NRFFirmwareUpdate();

    COPYABLE_MOVABLE(NRFFirmwareUpdate, delete);

    FirmwareUpdateDefs::ErrCode initUpdate(uint32_t len) override;
    FirmwareUpdateDefs::ErrCode writeFirmware(const uint8_t* pData, std::size_t len) override;
    FirmwareUpdateDefs::ErrCode runUpdate() override;

    void notification(BleDfuResponse response) final;

    void notification(BleEvent event) final;

    bool onManifest(NRFFirmwarePackageType type, size_t len);
    bool onFirmware(array_view<const uint8_t> data);
    bool onFirmwareEnd();

    size_t _bufferFull(const uint8_t* data, size_t size);

private:
    static constexpr uint32_t MAX_NUMBER_OF_RECEIVED_SLIP_PACKET = 2;

    IBleChip& mBleChip;
    QueueHandle_t mDfuPacketQueue;
    NRFFirmwarePackage mNRFFirmwarePackage;
    FlashWriteBuffer<uint8_t, DFU_PACKET_MAX_SIZE> mDfuDataBuffer;
    std::array<uint8_t, DFU_PACKET_NORMALIZE_SIZE> mResponsePayload;

    /* Current state */
    bool mIgnoreNextFirmware = false;
    size_t mCurrentObjectOffset = 0;
    size_t mCurrentObjectSize = 0;
    size_t mRemainingFirmwareSize = 0;
    size_t mCurrentFirmwareSize = 0;
    NRFFirmwarePackageType mCurrentFirmwareType = NRFFirmwarePackageType::FIRMWARE_UNKNOWN;
    Signal mEnteredDfuSignal;

    struct s_slipPacket
    {
        size_t len;
        uint8_t data[SLIP_DATA_MAX_SIZE];
    };

    bool ping(uint32_t nbRetry);
    bool setPrn();
    uint16_t getMtu();
    NRFExecuteStatus execute();
    uint32_t getVersion(NRFImageNumber image);
    bool waitEnteredDfu();

    bool send(BleDfuDataCmd& cmd, IDFUPacket& packet);
    bool receive(DFUResponsePacket& packet, uint32_t timeoutMs);
    bool sendAndWaitForReply(IDFUPacket& packet,
                             uint32_t timeout,
                             DFUResponsePacket& response,
                             DFUCommand responseCommand);
};

#endif /* NEMEIO_FIRMWAREUPDATE_UPDATER_NRFFIRMWAREUPDATE_HPP_ */
