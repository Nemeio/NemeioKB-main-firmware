/*
 * MainDisplayState.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_MAINDISPLAYSTATE_HPP_
#define DISPLAY_MAINDISPLAYSTATE_HPP_

#include "KeyboardDisplayState.hpp"
#include "NoDisplayVariant.hpp"
#include "ClearDisplayVariant.hpp"
#include "ConfigurationDisplayVariant.hpp"
#include "PairingDisplayVariant.hpp"

#include "utils.hpp"
#include "assertTools.h"

template<std::size_t CONFIGURATION_FILE_SIZE>
class MainDisplayState : public KeyboardDisplayState
{
public:
    template<class DisplayVariant>
    MainDisplayState(DisplayVariant& displayVariant);

    MainDisplayState();

    virtual ~MainDisplayState() = default;

    uint8_t apply(IScreen& screen, uint8_t fbNum) final;
    const ILayoutModifierStrategy& getModifierStrategy() const;

    bool operator==(const MainDisplayState& state) const;

private:
    etl::variant<NoDisplayVariant,
                 ClearDisplayVariant,
                 ConfigurationDisplayVariant<CONFIGURATION_FILE_SIZE>,
                 PairingDisplayVariant>
        mDisplayVariant;

    template<class DisplayVariant>
    bool compare(const etl::variant<NoDisplayVariant,
                                    ClearDisplayVariant,
                                    ConfigurationDisplayVariant<CONFIGURATION_FILE_SIZE>,
                                    PairingDisplayVariant>& comparedVariant) const;
};

template<std::size_t CONFIGURATION_FILE_SIZE>
template<class DisplayVariant>
MainDisplayState<CONFIGURATION_FILE_SIZE>::MainDisplayState(DisplayVariant& displayVariant)
    : mDisplayVariant(displayVariant)
{
}

template<std::size_t CONFIGURATION_FILE_SIZE>
uint8_t MainDisplayState<CONFIGURATION_FILE_SIZE>::apply(IScreen& screen, uint8_t fbNum)
{
    MainDisplayVariant& variant = castToBase<MainDisplayVariant>(mDisplayVariant);
    variant.draw(screen, fbNum);

    return fbNum;
}

template<std::size_t CONFIGURATION_FILE_SIZE>
const ILayoutModifierStrategy& MainDisplayState<CONFIGURATION_FILE_SIZE>::getModifierStrategy() const
{
    const MainDisplayVariant& variant = castToBase<MainDisplayVariant>(mDisplayVariant);
    return variant.getModifierStrategy();
}

template<std::size_t CONFIGURATION_FILE_SIZE>
bool MainDisplayState<CONFIGURATION_FILE_SIZE>::operator==(const MainDisplayState& state) const
{
    bool bRet = false;

    const MainDisplayVariant& comparedVariant = castToBase<MainDisplayVariant>(
        state.mDisplayVariant);
    const MainDisplayVariant& variant = castToBase<MainDisplayVariant>(mDisplayVariant);
    if(comparedVariant.isSameVariant(variant))
    {
        switch(variant.getId())
        {
        case MainDisplayVariant::MainDisplayVariantId::NO_DISPLAY:
            bRet = compare<NoDisplayVariant>(state.mDisplayVariant);
            break;
        case MainDisplayVariant::MainDisplayVariantId::CLEAR:
            bRet = compare<ClearDisplayVariant>(state.mDisplayVariant);
            break;
        case MainDisplayVariant::MainDisplayVariantId::CONFIGURATION:
            bRet = compare<ConfigurationDisplayVariant<CONFIGURATION_FILE_SIZE>>(
                state.mDisplayVariant);
            break;
        case MainDisplayVariant::MainDisplayVariantId::PAIRING:
            bRet = compare<PairingDisplayVariant>(state.mDisplayVariant);
            break;
        default:
            ASSERT(false);
            break;
        }
    }

    return bRet;
}

template<std::size_t CONFIGURATION_FILE_SIZE>
template<class DisplayVariant>
bool MainDisplayState<CONFIGURATION_FILE_SIZE>::compare(
    const etl::variant<NoDisplayVariant,
                       ClearDisplayVariant,
                       ConfigurationDisplayVariant<CONFIGURATION_FILE_SIZE>,
                       PairingDisplayVariant>& comparedVariant) const
{
    const DisplayVariant& comparedDisplay = etl::get<DisplayVariant>(comparedVariant);
    const DisplayVariant& display = etl::get<DisplayVariant>(mDisplayVariant);

    return comparedDisplay == display;
}

template<std::size_t CONFIGURATION_FILE_SIZE>
MainDisplayState<CONFIGURATION_FILE_SIZE>::MainDisplayState()
    : mDisplayVariant(NoDisplayVariant())
{
}


#endif /* DISPLAY_MAINDISPLAYSTATE_HPP_ */
