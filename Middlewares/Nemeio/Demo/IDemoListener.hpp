/*
 * IDemoListener.hpp
 *
 *  Created on: Nov 27, 2018
 *      Author: Labo
 */

#ifndef NEMEIO_DEMO_IDEMOLISTENER_HPP_
#define NEMEIO_DEMO_IDEMOLISTENER_HPP_

class IDemoListener
{
public:
    virtual ~IDemoListener() = default;

    virtual void demoStart() = 0;
    virtual void demoStop() = 0;
};

#endif /* NEMEIO_DEMO_IDEMOLISTENER_HPP_ */
