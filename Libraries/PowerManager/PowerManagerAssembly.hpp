/*
 * PowerManagerAssembly.hpp
 *
 *  Created on: 21 juil. 2020
 *      Author: eskoric
 */

#ifndef POWERMANAGER_POWERMANAGERASSEMBLY_HPP_
#define POWERMANAGER_POWERMANAGERASSEMBLY_HPP_

#include <array>
#include "PowerManager.h"
#include "PowerController.hpp"

template<std::size_t NB_POWERMANAGEABLES>
class PowerManagerAssembly
{
public:
    PowerManagerAssembly(ISystem& system,
                         PowerController& powerController,
                         std::array<IPowerManageable*, NB_POWERMANAGEABLES> powerManageables);

    PowerManager& getPowerManagerItem();

private:
    ListenerCollection<IPowerManageable*, NB_POWERMANAGEABLES> mPowerManageables;
    PowerManager mPowerManager;
};

template<std::size_t NB_POWERMANAGEABLES>
PowerManagerAssembly<NB_POWERMANAGEABLES>::PowerManagerAssembly(
    ISystem& system,
    PowerController& powerController,
    std::array<IPowerManageable*, NB_POWERMANAGEABLES> powerManageables)
    : mPowerManageables(powerManageables)
    , mPowerManager(system, powerController, mPowerManageables)
{
}

template<std::size_t NB_POWERMANAGEABLES>
PowerManager& PowerManagerAssembly<NB_POWERMANAGEABLES>::getPowerManagerItem()
{
    return mPowerManager;
}

#endif /* POWERMANAGER_POWERMANAGERASSEMBLY_HPP_ */
