/*
 * TAPLog.hpp
 *
 *  Created on: 10 mai 2021
 *      Author: thomas
 */

#ifndef BENCHMARK_TAPLOG_HPP_
#define BENCHMARK_TAPLOG_HPP_

#include "AbstractDataTransport.hpp"
#include "IDataTransportRxListener.hpp"
#include "ITestLog.hpp"
#include "embedded_ostream.hpp"

class TAPLog : public ITestLog
{
public:
    TAPLog(AbstractDataTransport& transport);
    virtual ~TAPLog() = default;

    void newTestSuite(etl::string_view name, size_t nbTest) final;
    void onSuccess(etl::string_view name, etl::string_view description) final;
    void onFailure(etl::string_view name, etl::string_view description) final;

private:
    AbstractDataTransport& mTransport;
    embedded_ostream mOsStream;
    size_t mCurrentTest = 0;
    size_t mNbTests = 0;

    void print(etl::string_view str);
    void increaseTestNumber();
};

#endif /* BENCHMARK_TAPLOG_HPP_ */
