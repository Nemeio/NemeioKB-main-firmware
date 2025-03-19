/*
 * InitDisplayStatus.cpp
 *
 *  Created on: Sep 24, 2019
 *      Author: Labo
 */

#include "InitDisplayStatus.hpp"

bool InitDisplayStatus::isInitialized() const
{
    return mCurrentState.isInitialized();
}
