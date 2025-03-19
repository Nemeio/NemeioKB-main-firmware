/*
 * Logger.hpp
 *
 *  Created on: May 22, 2019
 *      Author: Labo
 */

#ifndef TOOLS_LOGGER_LOGGERTOOL_HPP_
#define TOOLS_LOGGER_LOGGERTOOL_HPP_

#include "assertTools.h"
#include "Event.hpp"
#include "AbstractLogAdapter.hpp"

class Logger
{
public:
    explicit Logger(AbstractLogAdapter& eventLogAdapter);
    virtual ~Logger() = default;

    void checkForSysFailEvents();
    void logTestEvent(etl::string_view description);

private:
    AbstractLogAdapter& mEventLogAdapter;

    void addTestEvent(etl::string_view description);
    void addAssertionFailedEvent(const ArmRegisters& registers);
    void addFaultExceptionRaisedEvent(const ArmRegisters& registers, uint32_t exceptionType);
};

#endif /* TOOLS_LOGGER_LOGGERTOOL_HPP_ */
