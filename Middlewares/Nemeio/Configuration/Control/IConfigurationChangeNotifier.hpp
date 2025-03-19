/*
 * IConfigurationChangeNotifier.hpp
 *
 *  Created on: Feb 20, 2020
 *      Author: Labo
 */

#ifndef NEMEIO_CONFIGURATION_ICONFIGURATIONCHANGENOTIFIER_HPP_
#define NEMEIO_CONFIGURATION_ICONFIGURATIONCHANGENOTIFIER_HPP_

class IConfigurationChangeNotifier
{
public:
    IConfigurationChangeNotifier() = default;
    virtual ~IConfigurationChangeNotifier() = default;

    virtual void notifyConfigurationChanged(const Configuration* conf) = 0;
};

#endif /* NEMEIO_CONFIGURATION_ICONFIGURATIONCHANGENOTIFIER_HPP_ */
