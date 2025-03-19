#pragma once

#include "BleTxCmd.hpp"
#include "etl/vector.h"
#include "etl/array_view.h"

class BleHidInputReportCmd : public BleTxCmd
{
public:
    BleHidInputReportCmd();
    virtual ~BleHidInputReportCmd() = default;

    void setReportId(uint8_t reportId) { m_reportId = reportId; }
    void setInputReport(etl::array_view<const uint8_t> report)
    {
        m_report = etl::vector<uint8_t, MAX_INPUT_REPORT_SIZE>(report.begin(), report.end());
    }
    void fill(BleCmdPacket& packet) const final;

private:
    static constexpr size_t MAX_INPUT_REPORT_SIZE = 8;
    uint8_t m_reportId{0};
    etl::vector<uint8_t, MAX_INPUT_REPORT_SIZE> m_report;
};