/*
 * AbstractMutex.cpp
 *
 *  Created on: Nov 5, 2019
 *      Author: Labo
 */

#include "AbstractMutex.hpp"
#include "assertTools.h"

bool AbstractMutex::lock(DelayMs timeout)
{
    return doLock(timeout);
}
