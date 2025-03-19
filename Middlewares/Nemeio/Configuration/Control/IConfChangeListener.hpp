/*
 * IConfChangeListener.hpp
 *
 *  Created on: Feb 28, 2019
 *      Author: Labo
 */

#ifndef CONFCHANGELISTENER_ICONFCHANGELISTENER_HPP_TXT_
#define CONFCHANGELISTENER_ICONFCHANGELISTENER_HPP_TXT_

#include "Configuration.hpp"

class IConfChangeListener
{
public:
    IConfChangeListener() = default;
    virtual ~IConfChangeListener() = default;

    virtual void configurationChanged(const Configuration* pConfig) = 0;
};

#endif /* CONFCHANGELISTENER_ICONFCHANGELISTENER_HPP_TXT_ */
