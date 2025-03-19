/*
 * iNemeioKeypadPressedListener.h
 *
 *  Created on: 12 mai 2017
 *      Author: cgarnier
 */

#ifndef __INEMEIOKEYPADPRESSEDLISTENER_H_
#define __INEMEIOKEYPADPRESSEDLISTENER_H_

#include "Configuration.hpp"

class iNemeioKeypadPressedListener
{
public:
    iNemeioKeypadPressedListener() = default;
    virtual ~iNemeioKeypadPressedListener() = default;

    /*
	 *ButtonID.BTN_PRESSED = false -> key released
	 *ButtonID.BTN_PRESSED = true  -> key pressed
	 */
    virtual void pressedKey(const ButtonID& button) = 0;
    virtual void unpressedKey(const ButtonID& button) = 0;
    virtual void terminateUpdateKeys() = 0;
};

#endif /* __INEMEIOKEYPADPRESSEDLISTENER_H_ */
