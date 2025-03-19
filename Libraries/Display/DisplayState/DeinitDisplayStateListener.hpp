/*
 * DeinitDisplayStateListener.hpp
 *
 *  Created on: Sep 26, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_DEINITDISPLAYSTATELISTENER_HPP_
#define DISPLAY_DISPLAYSTATE_DEINITDISPLAYSTATELISTENER_HPP_

class DeinitDisplayStateListener
{
public:
    DeinitDisplayStateListener() = default;
    virtual ~DeinitDisplayStateListener() = default;

    virtual void displayDeinited() = 0;
};

#endif /* DISPLAY_DISPLAYSTATE_DEINITDISPLAYSTATELISTENER_HPP_ */
