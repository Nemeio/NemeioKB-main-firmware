#include "TestBenchId.hpp"

TestBenchId::TestBenchId(etl::string_view id)
    : m_id(id)
{
}

etl::string_view TestBenchId::get() const
{
    return m_id;
}

void TestBenchId::set(etl::string_view id)
{
    if(id.size() == m_id.capacity())
    {
        m_id.assign(id.begin(), id.end());
    }
}

bool TestBenchId::isValid() const
{
    return m_id.full();
}

bool TestBenchId::operator==(const TestBenchId& testBenchId) const
{
    return testBenchId.m_id == m_id;
}

bool TestBenchId::operator!=(const TestBenchId& testBenchId) const
{
    return !(testBenchId == *this);
}