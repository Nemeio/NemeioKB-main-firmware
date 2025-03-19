/*
 * TestSuite.hpp
 *
 *  Created on: 11 mai 2021
 *      Author: thomas
 */

#ifndef BENCHMARK_TESTSUITE_HPP_
#define BENCHMARK_TESTSUITE_HPP_

#include "ITestLog.hpp"
#include <vector>
#include <functional>
#include "embedded_ostream.hpp"

namespace benchmark
{
typedef std::function<bool(embedded_ostream&)> TestFunction;

class TestSuite
{
public:
    TestSuite() = delete;
    TestSuite(ITestLog& log);
    virtual ~TestSuite() = default;

    virtual bool run() = 0;

private:
    ITestLog& m_log;
    std::vector<std::pair<etl::string_view, TestFunction>> m_tests;

protected:
    void registerTest(etl::string_view name, TestFunction test);
    void unregisterAllTests();
    bool runTests(etl::string_view name);
};

} // namespace benchmark

#endif /* BENCHMARK_TESTSUITE_HPP_ */
