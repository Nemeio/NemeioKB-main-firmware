/*
 * PowerController.hpp
 *
 *  Created on: 16 juil. 2020
 *      Author: eskoric
 */

#ifndef POWERMANAGER_POWERCONTROLLER_HPP_
#define POWERMANAGER_POWERCONTROLLER_HPP_

#include "IPowerController.hpp"
#include "IApplication.hpp"

class PowerController : public IPowerController
{
public:
    explicit PowerController() = default;
    virtual ~PowerController() = default;

    void sleep();
    void shutdown();

private:
    void prepareWakeUpandShutdownSTM32();
};

#endif /* POWERMANAGER_POWERCONTROLLER_HPP_ */
