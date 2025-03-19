/*
 * SysFailLogDemo.hpp
 *
 *  Created on: Jun 11, 2019
 *      Author: Labo
 */

#ifndef LOG_SYSFAILLOGDEMO_HPP_
#define LOG_SYSFAILLOGDEMO_HPP_

#include "iNemeioKeypadPressedListener.h"
#include "keyIDConfig.h"

class SysFailLogDemo : public iNemeioKeypadPressedListener
{
public:
    SysFailLogDemo() = default;
    virtual ~SysFailLogDemo() = default;

    void pressedKey(const ButtonID& button);
    void unpressedKey(const ButtonID& button);
    void terminateUpdateKeys();

private:
    static const ButtonID BUTTON_ASSERTFAIL;
    static const ButtonID BUTTON_HARDFAULT;
};

#endif /* LOG_SYSFAILLOGDEMO_HPP_ */
