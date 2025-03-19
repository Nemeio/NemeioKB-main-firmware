#pragma once

#include "etl/array_view.h"

class IKeypadScanCodeConverter
{
public:
    IKeypadScanCodeConverter() = default;
    virtual ~IKeypadScanCodeConverter() = default;

    virtual void convertScanCodeToKeyUpdate(etl::array_view<const uint16_t> scanCodes) = 0;
};