/*
 * ValidateFunctionalTestsCmdObserver.hpp
 *
 *  Created on: 12 décembre 2022
 *      Author: Olivier
 */

#ifndef BENCHMARK_FUNCTIONAL_VALIDATEFUNCTIONALTESTSCMDOBSERVER_HPP_
#define BENCHMARK_FUNCTIONAL_VALIDATEFUNCTIONALTESTSCMDOBSERVER_HPP_

#include "etl/observer.h"
#include "TestBenchStatus.hpp"

namespace benchmark
{
using ExitFunctionalTestsCmdObserver = etl::observer<const TestBenchStatus&>;

} // namespace benchmark

#endif /* BENCHMARK_FUNCTIONAL_VALIDATEFUNCTIONALTESTSCMDOBSERVER_HPP_ */
