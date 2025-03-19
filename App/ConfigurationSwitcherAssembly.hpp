/*
 * ConfigurationSwitcherAssembly.hpp
 *
 *  Created on: Feb 20, 2020
 *      Author: Labo
 */

#ifndef NEMEIO_CONFIGURATION_CONFIGURATIONSWITCHERASSEMBLY_HPP_
#define NEMEIO_CONFIGURATION_CONFIGURATIONSWITCHERASSEMBLY_HPP_

#include "ConfigurationSwitcher.hpp"
#include "ConfigurationChangeNotifier.hpp"
#include "UserConfigurationNavigationFactory.hpp"

template<std::size_t NB_LISTENERS>
class ConfigurationSwitcherAssembly
{
public:
    ConfigurationSwitcherAssembly(IApplication& application,
                                  ConfigurationStorageContext& configurationStorageContext,
                                  ConfigurationStorageContext& factoryConfigurationStorageContext,
                                  std::array<IConfChangeListener*, NB_LISTENERS> confChangeListeners);
    virtual ~ConfigurationSwitcherAssembly() = default;
    ConfigurationSwitcher& getConfigurationSwitcher();
    const ConfigurationSwitcher& getConfigurationSwitcher() const;

private:
    UserConfigurationNavigationFactory mUserConfigurationNavigationFactory;
    ConfigurationChangeNotifier<NB_LISTENERS> mConfChangeNotifier;
    ConfigurationSwitcher mConfigurationSwitcher;
};

template<std::size_t NB_LISTENERS>
inline ConfigurationSwitcherAssembly<NB_LISTENERS>::ConfigurationSwitcherAssembly(
    IApplication& application,
    ConfigurationStorageContext& configurationStorageContext,
    ConfigurationStorageContext& factoryConfigurationStorageContext,
    std::array<IConfChangeListener*, NB_LISTENERS> confChangeListeners)
    : mConfChangeNotifier(confChangeListeners)
    , mConfigurationSwitcher(mUserConfigurationNavigationFactory,
                             application,
                             configurationStorageContext,
                             factoryConfigurationStorageContext,
                             mConfChangeNotifier)
{
}

template<std::size_t NB_LISTENERS>
ConfigurationSwitcher& ConfigurationSwitcherAssembly<NB_LISTENERS>::getConfigurationSwitcher()
{
    return mConfigurationSwitcher;
}

template<std::size_t NB_LISTENERS>
const ConfigurationSwitcher& ConfigurationSwitcherAssembly<NB_LISTENERS>::getConfigurationSwitcher()
    const
{
    return mConfigurationSwitcher;
}

#endif /* NEMEIO_CONFIGURATION_CONFIGURATIONSWITCHERASSEMBLY_HPP_ */
