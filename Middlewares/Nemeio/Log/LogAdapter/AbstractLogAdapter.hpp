/*
 * AbstractLogAdapter.hpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#ifndef LOG_LOGADAPTER_ABSTRACTLOGADAPTER_HPP_
#define LOG_LOGADAPTER_ABSTRACTLOGADAPTER_HPP_

#include <array>
#include "AbstractSerialAdapter.hpp"

enum class LogErrCode
{
    SUCCESS,
    NOMEM,
    STATE,
    ADAPTER,
    UNEXPECTED
};

class AbstractLogAdapter : public AbstractSerialAdapter
{
public:
    AbstractLogAdapter() = default;
    ~AbstractLogAdapter() override = default;

    virtual LogErrCode flush() = 0;
};

#endif /* LOG_LOGADAPTER_ABSTRACTLOGADAPTER_HPP_ */
