#pragma once

#include <etl/string.h>
#include <etl/string_view.h>

class TestBenchId
{
public:
    static constexpr size_t SIZE = 34;

    TestBenchId() = default;

    explicit TestBenchId(etl::string_view id);

    etl::string_view get() const;
    void set(etl::string_view id);

    bool isValid() const;

    bool operator==(const TestBenchId& testBenchId) const;
    bool operator!=(const TestBenchId& testBenchId) const;

private:
    etl::string<SIZE> m_id{""};
};