#pragma once

#include "gmock/gmock.h"
#include "ITraceLogger.hpp"

class MockTraceLogger : public logger::ITraceLogger
{
public:
    MOCK_METHOD(logger::TraceLoggerStream&, oss, (), (override));
    MOCK_METHOD(void, setError, (), (override));
    MOCK_METHOD(void, clearError, (), (override));
    MOCK_METHOD(bool, isErrorLogged, (), (const override));
    MOCK_METHOD(bool, lock, (DelayMs timeout), (override));
    MOCK_METHOD(bool, unlock, (), (override));
};