#include "BleHidOutputReportCmd.hpp"

BleHidOutputReportCmd::BleHidOutputReportCmd()
    : AbstractBleCommandListener(BLESPICommands::CommandId::OUTPUT_REPORT)
{
}

void BleHidOutputReportCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    notify_observers(BleHidOutputReportEvent{{pData, dataLen}});
}