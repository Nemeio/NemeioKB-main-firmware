/*
 * MainDisplayStatus.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_MAINDISPLAYSTATUS_HPP_
#define DISPLAY_DISPLAYSTATE_MAINDISPLAYSTATUS_HPP_

#include "KeyboardDisplayStatus.hpp"
#include "MainDisplayState.hpp"
#include "ILayoutModifierStrategy.hpp"

template<std::size_t CONFIGURATION_FILE_SIZE>
class MainDisplayStatus : public KeyboardDisplayStatus<MainDisplayState<CONFIGURATION_FILE_SIZE>>
{
public:
    MainDisplayStatus() = default;
    virtual ~MainDisplayStatus() = default;

    const ILayoutModifierStrategy& getCurrentStateModifierStrategy();
};

template<std::size_t CONFIGURATION_FILE_SIZE>
inline const ILayoutModifierStrategy&
MainDisplayStatus<CONFIGURATION_FILE_SIZE>::getCurrentStateModifierStrategy()
{
    return this->mCurrentState.getModifierStrategy();
}

#endif /* DISPLAY_DISPLAYSTATE_MAINDISPLAYSTATUS_HPP_ */
