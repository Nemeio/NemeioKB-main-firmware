/*
 * TestLog.hpp
 *
 *  Created on: 10 mai 2021
 *      Author: thomas
 */

#ifndef BENCHMARK_ITESTLOG_HPP_
#define BENCHMARK_ITESTLOG_HPP_

#include <etl/string_view.h>

class ITestLog
{
public:
    virtual void newTestSuite(etl::string_view name, size_t nbTest) = 0;
    virtual void onSuccess(etl::string_view name, etl::string_view description) = 0;
    virtual void onFailure(etl::string_view name, etl::string_view description) = 0;
};


#endif /* BENCHMARK_ITESTLOG_HPP_ */
