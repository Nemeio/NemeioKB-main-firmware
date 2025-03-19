/*
 * ConfigurationChangeNotifier.hpp
 *
 *  Created on: Feb 20, 2020
 *      Author: Labo
 */

#ifndef NEMEIO_CONFIGURATION_CONFIGURATIONCHANGENOTIFIER_HPP_
#define NEMEIO_CONFIGURATION_CONFIGURATIONCHANGENOTIFIER_HPP_

#include "IConfigurationChangeNotifier.hpp"
#include "IConfChangeListener.hpp"
#include "ListenerCollection.hpp"

template<std::size_t N>
class ConfigurationChangeNotifier : public IConfigurationChangeNotifier
{
public:
    ConfigurationChangeNotifier(std::array<IConfChangeListener*, N> confChangeListeners);
    virtual ~ConfigurationChangeNotifier() = default;

    void notifyConfigurationChanged(const Configuration* pConf);

private:
    ListenerCollection<IConfChangeListener*, N> mConfChangeListeners;
};

template<std::size_t N>
ConfigurationChangeNotifier<N>::ConfigurationChangeNotifier(
    std::array<IConfChangeListener*, N> confChangeListeners)
    : mConfChangeListeners(confChangeListeners)
{
}

template<std::size_t N>
inline void ConfigurationChangeNotifier<N>::notifyConfigurationChanged(const Configuration* pConf)
{
    mConfChangeListeners.notify(
        [pConf](IConfChangeListener* listener)
        {
            if(listener != nullptr)
            {
                listener->configurationChanged(pConf);
            }
        });
}

#endif /* NEMEIO_CONFIGURATION_CONFIGURATIONCHANGENOTIFIER_HPP_ */
