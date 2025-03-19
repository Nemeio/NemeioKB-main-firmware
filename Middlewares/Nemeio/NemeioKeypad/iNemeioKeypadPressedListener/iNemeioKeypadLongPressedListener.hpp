/*
 * iNemeioKeypadLongPressedListener.hpp
 *
 *  Created on: Sep 10, 2018
 *      Author: Labo
 */

#ifndef NEMEIO_NEMEIOKEYPAD_INEMEIOKEYPADPRESSEDLISTENER_INEMEIOKEYPADLONGPRESSEDLISTENER_HPP_
#define NEMEIO_NEMEIOKEYPAD_INEMEIOKEYPADPRESSEDLISTENER_INEMEIOKEYPADLONGPRESSEDLISTENER_HPP_

#include "iNemeioKeypadPressedListener.h"

class iNemeioKeypadLongPressedListener : public iNemeioKeypadPressedListener
{
public:
    iNemeioKeypadLongPressedListener() = default;
    virtual ~iNemeioKeypadLongPressedListener() = default;

    virtual void longPressedKey(const ButtonID& button) = 0;
};

#endif /* NEMEIO_NEMEIOKEYPAD_INEMEIOKEYPADPRESSEDLISTENER_INEMEIOKEYPADLONGPRESSEDLISTENER_HPP_ */
