#pragma once
#include "AppCommCmd.hpp"
#include "BleChip.hpp"
#include "TestBenchId.hpp"
#include "utils.hpp"

namespace benchmark
{
class GetTestBenchIdCmd : public AppCommCmd
{
public:
    GetTestBenchIdCmd(IAppCommTxListener& appCommTxListener, const TestBenchId& testBenchId);
    ~GetTestBenchIdCmd() final = default;

private:
    PACK((struct Response
          {
              AppCommDefs::AppCommErrCode retCode;
              char id[TestBenchId::SIZE];
          }));

    const TestBenchId& m_testBenchId;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
};

} // namespace benchmark
