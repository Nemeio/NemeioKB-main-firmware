#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <vector>

#include "AbstractSerialAdapter.hpp"
#include "FaultExceptionEvent.hpp"
#include "AssertFailEvent.hpp"
#include "TestEvent.hpp"

class EventLogTest : public ::testing::Test
{
protected:
    EventLogTest() = default;

    class StubSerialAdapter : public AbstractSerialAdapter
    {
    public:
        const std::vector<uint8_t> getData() { return m_data; }

    private:
        std::size_t writeBuffer(const uint8_t* pData, std::size_t len) final
        {
            m_data.insert(m_data.end(), pData, pData + len);

            return len;
        }
        std::vector<uint8_t> m_data;
    };
};

TEST_F(EventLogTest, FaultExceptionEvent)
{
    StubSerialAdapter adapter;

    ArmRegisters registers{0xABCDEF01,
                           0x02,
                           0x03,
                           0x04,
                           0x05,
                           0x06,
                           0x07,
                           0x08,
                           0x09,
                           0x0A,
                           0x0B,
                           0x0C,
                           0x0D,
                           0x0E,
                           0x0F,
                           0x10,
                           0xAABBCC11};

    FaultExceptionEvent event(registers, 0xDDEEFF04);

    std::vector<uint8_t> expectedData = {0x00, 0x01, 0xEF, 0xCD, 0xAB, 0x02, 0x00, 0x00, 0x00, 0x03,
                                         0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
                                         0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08,
                                         0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00,
                                         0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0D,
                                         0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00,
                                         0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0xCC, 0xBB, 0xAA, 0x04,
                                         0xFF, 0xEE, 0xDD};

    adapter << event;
    EXPECT_EQ(adapter.getData(), expectedData);
}

TEST_F(EventLogTest, AssertFailEvent)
{
    StubSerialAdapter adapter;

    ArmRegisters registers{0xABCDEF01,
                           0x02,
                           0x03,
                           0x04,
                           0x05,
                           0x06,
                           0x07,
                           0x08,
                           0x09,
                           0x0A,
                           0x0B,
                           0x0C,
                           0x0D,
                           0x0E,
                           0x0F,
                           0x10,
                           0xAABBCC11};

    AssertFailEvent event(registers);

    std::vector<uint8_t> expectedData = {0x01, 0x01, 0xEF, 0xCD, 0xAB, 0x02, 0x00, 0x00, 0x00, 0x03,
                                         0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
                                         0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08,
                                         0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00,
                                         0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0D,
                                         0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00,
                                         0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0xCC, 0xBB, 0xAA};

    adapter << event;
    EXPECT_EQ(adapter.getData(), expectedData);
}

TEST_F(EventLogTest, TestEvent)
{
    StubSerialAdapter adapter;

    TestEvent event("desc");
    std::vector<uint8_t> expectedData = {0x02, 0x04, 0x00, 0x00, 0x00, 'd', 'e', 's', 'c'};

    adapter << event;
    EXPECT_EQ(adapter.getData(), expectedData);
}