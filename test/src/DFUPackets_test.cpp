#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "DFUPingPacket.hpp"
#include "DFUExecutePacket.hpp"
#include "DFUCreateObjectPacket.hpp"
#include "DFUMTUPacket.hpp"
#include "DFUSetReceipeNotificationPacket.hpp"
#include "DFUGetFirmwareVersionPacket.hpp"
#include "DFUWriteObjectPacket.hpp"
#include "DFUResponsePacket.hpp"
#include "DFUFirmwareVersionResponsePacket.hpp"

using ::testing::NiceMock;
using ::testing::StrictMock;

class DFUPacketsTest : public ::testing::Test
{
protected:
    DFUPacketsTest(){};
};

TEST_F(DFUPacketsTest, TooBigData)
{
    std::array<uint8_t, 20> randomData;
    DFUPacket<10> packet(array_view<const uint8_t>(randomData.data(), randomData.size()));

    EXPECT_EQ(packet.size(), 0);
};

TEST_F(DFUPacketsTest, TooBigPayload){};

TEST_F(DFUPacketsTest, PingPacket)
{
    DFUPingPacket ping(5);
    std::array<uint8_t, 5> pingData = {9, 5, 0, 0, 0};
    EXPECT_EQ(ping.size(), pingData.size());
    EXPECT_EQ(ping.getCommand(), DFUCommand::PING);
    EXPECT_EQ(memcmp(ping.data(), pingData.data(), ping.size()), 0);
};

TEST_F(DFUPacketsTest, Execute)
{
    DFUExecutePacket execute{};
    std::array<uint8_t, 1> executeData = {4};
    EXPECT_EQ(execute.size(), executeData.size());
    EXPECT_EQ(execute.getCommand(), DFUCommand::EXECUTE);
    EXPECT_EQ(memcmp(execute.data(), executeData.data(), execute.size()), 0);
};

TEST_F(DFUPacketsTest, CreateInitObjectPacket)
{
    DFUCreateObjectPacket createObject(DFUObjectType::FIRMWARE_INIT, 0x400);
    std::array<uint8_t, 6> createObjectData = {1, 1, 0x0, 0x4, 0x00, 0x00};
    EXPECT_EQ(createObject.size(), createObjectData.size());
    EXPECT_EQ(createObject.getCommand(), DFUCommand::CREATE_OBJECT);
    EXPECT_EQ(memcmp(createObject.data(), createObjectData.data(), createObject.size()), 0);
};

TEST_F(DFUPacketsTest, CreateDataObjectPacket)
{
    DFUCreateObjectPacket createObject(DFUObjectType::FIRMWARE_DATA, 0x400);
    std::array<uint8_t, 6> createObjectData = {1, 2, 0x0, 0x4, 0x00, 0x00};
    EXPECT_EQ(createObject.size(), createObjectData.size());
    EXPECT_EQ(createObject.getCommand(), DFUCommand::CREATE_OBJECT);
    EXPECT_EQ(memcmp(createObject.data(), createObjectData.data(), createObject.size()), 0);
};

TEST_F(DFUPacketsTest, MTU)
{
    DFUMTUPacket mtu{};
    std::array<uint8_t, 1> mtuData = {7};
    EXPECT_EQ(mtu.size(), mtuData.size());
    EXPECT_EQ(mtu.getCommand(), DFUCommand::GET_SERIAL_MTU);
    EXPECT_EQ(memcmp(mtu.data(), mtuData.data(), mtu.size()), 0);
};

TEST_F(DFUPacketsTest, SetReceipeNotification)
{
    DFUSetReceipeNotificationPacket prn(25);
    std::array<uint8_t, 5> prnData = {2, 25, 0, 0, 0};
    EXPECT_EQ(prn.size(), prnData.size());
    EXPECT_EQ(prn.getCommand(), DFUCommand::SET_PRN);
    EXPECT_EQ(memcmp(prn.data(), prnData.data(), prn.size()), 0);
};

TEST_F(DFUPacketsTest, GetFirmwareVersionPacket)
{
    DFUGetFirmwareVersionPacket firmwareVersion(1);
    std::array<uint8_t, 2> firmwareVersionData = {11, 1};
    EXPECT_EQ(firmwareVersion.size(), firmwareVersionData.size());
    EXPECT_EQ(firmwareVersion.getCommand(), DFUCommand::GET_FIRMWARE_VERSION);
    EXPECT_EQ(memcmp(firmwareVersion.data(), firmwareVersionData.data(), firmwareVersion.size()), 0);
};

TEST_F(DFUPacketsTest, WriteObject)
{
    const std::array<uint8_t, 15> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    DFUWriteObjectPacket<20> writeObject(array_view<const uint8_t>(data.data(), data.size()));
    std::array<uint8_t, 16> writeObjectData = {8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    EXPECT_EQ(writeObject.size(), writeObjectData.size());
    EXPECT_EQ(writeObject.getCommand(), DFUCommand::WRITE_OBJECT);
    EXPECT_EQ(memcmp(writeObject.data(), writeObjectData.data(), writeObject.size()), 0);
};

TEST_F(DFUPacketsTest, WriteObjectTooBig)
{
    const std::array<uint8_t, 256> data = {0};
    DFUWriteObjectPacket<20> writeObject(array_view<const uint8_t>(data.data(), data.size()));
    EXPECT_EQ(writeObject.size(), 0);
};

TEST_F(DFUPacketsTest, FirmwareResponsePacket)
{
    std::array<uint8_t, 16> data = {96, 11, 1, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    DFUFirmwareVersionResponsePacket responsePacket;
    responsePacket.setData(array_view<const uint8_t>(data.data(), data.size()));

    EXPECT_EQ(responsePacket.getCommand(), DFUCommand::GET_FIRMWARE_VERSION);
    EXPECT_EQ(responsePacket.isValidResponse(), true);
    EXPECT_EQ(responsePacket.getResponseCode(), DFUResponseCode::SUCCESS);

    EXPECT_EQ(responsePacket.getType(), 2);
    EXPECT_EQ(responsePacket.getVersion(), 0x04030201);
    EXPECT_EQ(responsePacket.getStartAddress(), 0x08070605);
    EXPECT_EQ(responsePacket.getSize(), 0x0c0b0a09);
};
