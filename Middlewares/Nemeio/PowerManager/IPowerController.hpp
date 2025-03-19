/*
 * IPowerController.hpp
 *
 *  Created on: 16 juil. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_POWERMANAGER_IPOWERCONTROLLER_HPP_
#define NEMEIO_POWERMANAGER_IPOWERCONTROLLER_HPP_

class IPowerController
{
public:
    IPowerController() = default;
    virtual ~IPowerController() = default;

    virtual void sleep() = 0;
    virtual void shutdown() = 0;
};

#endif /* NEMEIO_POWERMANAGER_IPOWERCONTROLLER_HPP_ */
