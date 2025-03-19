/*
 * MainDisplayVariant.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_MAINDISPLAYVARIANT_HPP_
#define DISPLAY_MAINDISPLAYVARIANT_HPP_

#include "NoLayoutModifierStrategy.hpp"
#include "IScreen.hpp"

class MainDisplayVariant
{
public:
    enum class MainDisplayVariantId
    {
        NO_DISPLAY,
        CLEAR,
        CONFIGURATION,
        PAIRING
    };

    MainDisplayVariant(MainDisplayVariantId id);
    virtual ~MainDisplayVariant() = default;

    bool isSameVariant(const MainDisplayVariant& variant) const;
    MainDisplayVariantId getId() const;
    virtual const ILayoutModifierStrategy& getModifierStrategy() const;
    virtual void draw(IScreen& screen, uint8_t fbNum) = 0;

private:
    MainDisplayVariantId mId;
    NoLayoutModifierStrategy mNoModifierStrategy;
};

#endif /* DISPLAY_MAINDISPLAYVARIANT_HPP_ */
