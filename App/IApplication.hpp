/*
 * IApplication.hpp
 *
 *  Created on: Jan 13, 2020
 *      Author: Labo
 */

#ifndef IAPPLICATION_HPP_
#define IAPPLICATION_HPP_

#include "Configuration.hpp"
#include "KeyboardParameters.hpp"
#include "ILayoutStorage.hpp"

enum class AppErrCode
{
    SUCCESS,
    UNEXPECTED,
    FS,
    INVALIDDATA,
    PROTECTLEVEL,
    DRAWCFG,
    NOTFOUND,
    WRONG_COMM_MODE,
};

class IApplication
{
public:
    IApplication() = default;
    virtual ~IApplication() = default;
    virtual bool applyLayoutConfiguration(
        ILayoutStorage& storage, const configuration::LayoutConfiguration& layoutConfiguration) = 0;
    virtual KeyboardParameters getKeyboardParam() = 0;
    virtual void setKeyboardParam(const KeyboardParameters& params) = 0;
    virtual void factoryReset() = 0;
    virtual void FormatFlash() = 0;
};

#endif /* IAPPLICATION_HPP_ */
