#include "DevicePropertiesBlockDeviceStorage.hpp"
#include "assertTools.h"
#include "etl_string_utils.hpp"

DevicePropertiesBlockDeviceStorage::DevicePropertiesBlockDeviceStorage(IBlockDevice& blockDevice)
    : m_blockDevice(blockDevice)
{
}

bool DevicePropertiesBlockDeviceStorage::saveTestBenchId(const TestBenchId& testBenchId)
{
    ASSERT(m_blockDevice.size() >= sizeof(s_deviceProperties));

    if(bd_size_t erase_size = m_blockDevice.get_erase_size(0); erase_size != 0)
    {
        m_blockDevice.erase(0, erase_size);
    }

    s_devicePropertiesFirstBlockTestBenchId firstBlock = {0};

    etl::string_view testBenchIdStr = testBenchId.get();
    etl::copy(testBenchIdStr.begin(), testBenchIdStr.end(), firstBlock.testBenchId);

    firstBlock.magicNumber = DEVICE_PROPERTIES_MAGIC_NUMBER_FIRST_BLOCK;
    int ret = m_blockDevice.program(&firstBlock, 0, sizeof(firstBlock));

    return (ret == BD_ERROR_OK);
}


bool DevicePropertiesBlockDeviceStorage::saveProvisioningData(const SerialNumber& serialNumber,
                                                              const MacAddress& macAddress)
{
    // First block (test bench id) must be written first
    if(!isFirstBlockTestBenchIdStored())
    {
        return false;
    }

    s_devicePropertiesProvisioningBlock provisioningBlock = {0};

    etl::string_view serialNumberStr = serialNumber.getString();
    etl::copy(serialNumberStr.begin(), serialNumberStr.end(), provisioningBlock.serialNumber);
    etl::array_view<const uint8_t> macAddressBuffer = macAddress.get();
    etl::copy(macAddressBuffer.begin(), macAddressBuffer.end(), provisioningBlock.bleMacAddress);

    provisioningBlock.magicNumber = DEVICE_PROPERTIES_MAGIC_NUMBER_PROVISIONING_BLOCK;
    int ret = m_blockDevice.program(&provisioningBlock,
                                    getAbsoluteOffset(
                                        offsetof(s_deviceProperties, provisioningBlock)),
                                    sizeof(provisioningBlock));

    return (ret == BD_ERROR_OK);
}

bool DevicePropertiesBlockDeviceStorage::loadTestBenchId(TestBenchId& retTestBenchId)
{
    bool ret = false;
    size_t offsetInStruct = offsetof(s_deviceProperties, firstBlockTestBenchId)
                            + offsetof(s_devicePropertiesFirstBlockTestBenchId, testBenchId);

    if(etl::array<char, TestBenchId::SIZE> testBenchIdBuffer;
       isFirstBlockTestBenchIdStored()
       && m_blockDevice.read(testBenchIdBuffer.data(),
                             getAbsoluteOffset(offsetInStruct),
                             testBenchIdBuffer.size())
              == 0)
    {
        retTestBenchId = TestBenchId(etl_string_utils::arrayToStringView(
            {testBenchIdBuffer.begin(), testBenchIdBuffer.end()}));
        ret = true;
    }

    return ret;
}
bool DevicePropertiesBlockDeviceStorage::loadSerialNumber(SerialNumber& retSerialNumber)
{
    bool ret = false;
    size_t offsetInStruct = offsetof(s_deviceProperties, provisioningBlock)
                            + offsetof(s_devicePropertiesProvisioningBlock, serialNumber);

    if(etl::array<char, SerialNumber::SIZE> serialNumberBuffer;
       isProvisioningBlockStored()
       && m_blockDevice.read(serialNumberBuffer.data(), offsetInStruct, serialNumberBuffer.size())
              == 0)
    {
        retSerialNumber = SerialNumber(etl_string_utils::arrayToStringView(
            {serialNumberBuffer.begin(), serialNumberBuffer.end()}));
        ret = true;
    }

    return ret;
}

bool DevicePropertiesBlockDeviceStorage::loadMacAddress(MacAddress& retMacAddress)
{
    bool ret = false;
    size_t offsetInStruct = offsetof(s_deviceProperties, provisioningBlock)
                            + offsetof(s_devicePropertiesProvisioningBlock, bleMacAddress);

    if(etl::array<uint8_t, MacAddress::SIZE> bleMacAddressBuffer;
       isProvisioningBlockStored()
       && m_blockDevice.read(bleMacAddressBuffer.data(), offsetInStruct, bleMacAddressBuffer.size())
              == 0)
    {
        retMacAddress = MacAddress(bleMacAddressBuffer);
        ret = true;
    }

    return ret;
}

uint32_t DevicePropertiesBlockDeviceStorage::getAbsoluteOffset(uint32_t offset) const
{
    return DEVICE_PROPERTIES_FLASH_OFFSET + offset;
}

bool DevicePropertiesBlockDeviceStorage::isProvisioningBlockStored()
{
    uint32_t magicNumber = 0;
    if(size_t offsetInStruct = offsetof(s_deviceProperties, provisioningBlock)
                               + offsetof(s_devicePropertiesProvisioningBlock, magicNumber);
       m_blockDevice.read(&magicNumber, getAbsoluteOffset(offsetInStruct), sizeof(magicNumber)) != 0)
    {
        magicNumber = 0;
    }

    return (magicNumber == DEVICE_PROPERTIES_MAGIC_NUMBER_PROVISIONING_BLOCK);
}


bool DevicePropertiesBlockDeviceStorage::isFirstBlockTestBenchIdStored()
{
    uint32_t magicNumber = 0;
    if(size_t offsetInStruct = offsetof(s_deviceProperties, firstBlockTestBenchId)
                               + offsetof(s_devicePropertiesFirstBlockTestBenchId, magicNumber);
       m_blockDevice.read(&magicNumber, getAbsoluteOffset(offsetInStruct), sizeof(magicNumber)) != 0)
    {
        magicNumber = 0;
    }

    return (magicNumber == DEVICE_PROPERTIES_MAGIC_NUMBER_FIRST_BLOCK);
}