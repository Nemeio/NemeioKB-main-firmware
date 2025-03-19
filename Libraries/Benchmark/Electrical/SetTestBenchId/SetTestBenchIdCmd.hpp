#pragma once
#include "AppCommCmd.hpp"
#include "BleChip.hpp"
#include "TestBenchId.hpp"

namespace benchmark
{
class SetTestBenchIdCmd : public AppCommCmd
{
public:
    SetTestBenchIdCmd(IAppCommTxListener& appCommTxListener, TestBenchId& testBenchId);
    ~SetTestBenchIdCmd() final = default;

private:
    TestBenchId& m_testBenchId;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
};

} // namespace benchmark
