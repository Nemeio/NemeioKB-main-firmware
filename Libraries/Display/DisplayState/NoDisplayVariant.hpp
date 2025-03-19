/*
 * NoDisplayState.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_NODISPLAYVARIANT_HPP_
#define DISPLAY_DISPLAYSTATE_NODISPLAYVARIANT_HPP_

#include "MainDisplayVariant.hpp"

class NoDisplayVariant : public MainDisplayVariant
{
public:
    NoDisplayVariant();
    virtual ~NoDisplayVariant() = default;

    void draw(IScreen& screen, uint8_t fbNum);

    bool operator==(const NoDisplayVariant& variant) const;
};

#endif /* DISPLAY_DISPLAYSTATE_NODISPLAYVARIANT_HPP_ */
