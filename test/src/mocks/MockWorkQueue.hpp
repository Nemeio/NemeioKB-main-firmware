#pragma once

#include "gmock/gmock.h"
#include "IWorkQueue.hpp"

class MockWorkQueue : public IWorkQueue
{
public:
    MOCK_METHOD(bool, submit, (Work & work), (override));
    MOCK_METHOD(bool, submitData, (Work & work, etl::array_view<const uint8_t> data), (override));
};
