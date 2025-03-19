#pragma once

#include "IDevicePropertiesStorage.hpp"
#include "IBlockDevice.hpp"
#include "utils.hpp"

class DevicePropertiesBlockDeviceStorage : public IDevicePropertiesStorage
{
public:
    explicit DevicePropertiesBlockDeviceStorage(IBlockDevice& blockDevice);
    ~DevicePropertiesBlockDeviceStorage() final = default;

    bool saveTestBenchId(const TestBenchId& testBenchId) final;
    bool saveProvisioningData(const SerialNumber& serialNumber, const MacAddress& macAddress) final;
    bool loadTestBenchId(TestBenchId& testBenchId) final;
    bool loadSerialNumber(SerialNumber& serialNumber) final;
    bool loadMacAddress(MacAddress& macAddress) final;

private:
    static constexpr uint32_t DEVICE_PROPERTIES_MAGIC_NUMBER_FIRST_BLOCK = 0x44494254; // ASCII "TBID"
    static constexpr uint32_t DEVICE_PROPERTIES_MAGIC_NUMBER_PROVISIONING_BLOCK = 0xBAADF00D;
    static constexpr uint32_t DEVICE_PROPERTIES_FLASH_OFFSET = 0;

    PACK_ALIGN((struct s_devicePropertiesFirstBlockTestBenchId
                {
                    PACK((struct
                          {
                              uint8_t reserved;
                              uint32_t magicNumber;
                              char testBenchId[TestBenchId::SIZE];
                          }));
                }),
               sizeof(uint64_t));

    PACK_ALIGN((struct s_devicePropertiesProvisioningBlock
                {
                    PACK((struct
                          {
                              uint32_t magicNumber;
                              uint8_t bleMacAddress[MacAddress::SIZE];
                              char serialNumber[SerialNumber::SIZE];
                          }));
                }),
               sizeof(uint64_t));

    /* How data are stored in block device */
    PACK_ALIGN((struct s_deviceProperties
                {
                    s_devicePropertiesFirstBlockTestBenchId firstBlockTestBenchId;
                    s_devicePropertiesProvisioningBlock provisioningBlock;
                }),
               sizeof(uint64_t));

    IBlockDevice& m_blockDevice;

    uint32_t getAbsoluteOffset(uint32_t offset) const;

    bool isProvisioningBlockStored();
    bool isFirstBlockTestBenchIdStored();
};