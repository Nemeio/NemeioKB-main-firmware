/*
 * DummyLog.hpp
 *
 *  Created on: 22 sept. 2021
 *      Author: thomas
 */

#ifndef BENCHMARK_DUMMYLOG_HPP_
#define BENCHMARK_DUMMYLOG_HPP_

#include "ITestLog.hpp"

class DummyLog : public ITestLog
{
public:
    DummyLog() = default;
    virtual ~DummyLog() = default;

    void newTestSuite(etl::string_view name, size_t nbTest) final;
    void onSuccess(etl::string_view name, etl::string_view description) final;
    void onFailure(etl::string_view name, etl::string_view description) final;
};

#endif /* BENCHMARK_DUMMYLOG_HPP_ */
