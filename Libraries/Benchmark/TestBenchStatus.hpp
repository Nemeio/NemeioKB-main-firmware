#pragma once

namespace benchmark
{
enum class TestBenchStatus : uint8_t
{
    INVALIDATE = 0,
    VALIDATE,
    IGNORE,
    MAX
};
} // namespace benchmark