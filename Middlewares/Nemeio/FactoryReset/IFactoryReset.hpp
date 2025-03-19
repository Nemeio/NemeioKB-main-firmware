/*
 * IFactoryReset.hpp
 *
 *  Created on: Dec 6, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_FACTORYRESET_IFACTORYRESET_HPP_
#define NEMEIO_FACTORYRESET_IFACTORYRESET_HPP_

#include <functional>

class IFactoryReset
{
public:
    IFactoryReset() = default;
    virtual ~IFactoryReset() = default;

    enum class ErrCode
    {
        SUCCESS,
        BLE
    };

    virtual void run(const std::function<void(ErrCode)>& actionBeforeSystemReset) = 0;
};

#endif /* NEMEIO_FACTORYRESET_IFACTORYRESET_HPP_ */
