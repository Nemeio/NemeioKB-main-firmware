#pragma once

#include "BleTxCmd.hpp"
#include "etl/array_view.h"

class BleHidReportDescriptorCmd : public BleTxCmd
{
public:
    BleHidReportDescriptorCmd();
    virtual ~BleHidReportDescriptorCmd() = default;

    void setReportLengths(etl::array_view<const uint16_t> reportLengths)
    {
        if(reportLengths.size() <= MAX_NB_REPORTS)
        {
            m_reportLengths = etl::vector<uint16_t, MAX_NB_REPORTS>(reportLengths.begin(),
                                                                    reportLengths.end());
        }
    }
    // Report map must point to static data
    void setReportDescriptor(etl::array_view<const uint8_t> reportMap) { m_reportMap = reportMap; }
    void fill(BleCmdPacket& packet) const final;

private:
    static constexpr uint8_t MAX_NB_REPORTS = 3;

    etl::vector<uint16_t, MAX_NB_REPORTS> m_reportLengths;
    etl::array_view<const uint8_t> m_reportMap;
};