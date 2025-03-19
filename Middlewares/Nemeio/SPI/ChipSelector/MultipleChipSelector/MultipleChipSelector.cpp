/*
 * MultipleChipSelector.cpp
 *
 *  Created on: May 12, 2017
 *      Author: Labo
 */

#include "MultipleChipSelector.h"

using namespace std;

MultipleChipSelector::MultipleChipSelector()
{
}

MultipleChipSelector::~MultipleChipSelector()
{
}

void MultipleChipSelector::activate()
{
    for(vector<iChipSelector*>::iterator it = mVChipSelectors.begin(); it != mVChipSelectors.end();
        it++)
    {
        (*it)->activate();
    }
}

void MultipleChipSelector::deactivate()
{
    for(vector<iChipSelector*>::iterator it = mVChipSelectors.begin(); it != mVChipSelectors.end();
        it++)
    {
        (*it)->deactivate();
    }
}

void MultipleChipSelector::add(iChipSelector& chipSelector)
{
    mVChipSelectors.push_back(&chipSelector);
}

void MultipleChipSelector::enable()
{
}

void MultipleChipSelector::disable()
{
}
