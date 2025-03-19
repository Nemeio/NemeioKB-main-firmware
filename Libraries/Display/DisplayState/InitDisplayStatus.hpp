/*
 * InitDisplayStatus.hpp
 *
 *  Created on: Sep 24, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_INITDISPLAYSTATUS_HPP_
#define DISPLAY_DISPLAYSTATE_INITDISPLAYSTATUS_HPP_

#include "KeyboardDisplayStatus.hpp"
#include "InitDisplayState.hpp"

class InitDisplayStatus : public KeyboardDisplayStatus<InitDisplayState>
{
public:
    InitDisplayStatus() = default;
    virtual ~InitDisplayStatus() = default;

    bool isInitialized() const;
};

#endif /* DISPLAY_DISPLAYSTATE_INITDISPLAYSTATUS_HPP_ */
