/*
 * TestSuite.cpp
 *
 *  Created on: 18 mai 2021
 *      Author: thomas
 */

#include "TestSuite.hpp"

using namespace benchmark;

TestSuite::TestSuite(ITestLog& log)
    : m_log(log){};

void TestSuite::registerTest(etl::string_view name, TestFunction test)
{
    m_tests.push_back(std::make_pair(name, test));
}

void TestSuite::unregisterAllTests()
{
    m_tests.clear();
}

bool TestSuite::runTests(etl::string_view name)
{
    bool ret = true;
    embedded_ostream description;

    m_log.newTestSuite(name, m_tests.size());

    for(auto& test: m_tests)
    {
        description.clear();
        if(test.second(description))
        {
            m_log.onSuccess(test.first, description.str());
        }
        else
        {
            m_log.onFailure(test.first, description.str());
            ret = false;
        }
    }

    return ret;
}
