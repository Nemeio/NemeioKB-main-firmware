/*
 * ScreenFactory.hpp
 *
 *  Created on: 19 oct. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_SCREENFACTORY_SCREENFACTORY_HPP_
#define NEMEIO_DISPLAY_SCREENFACTORY_SCREENFACTORY_HPP_

#include "etl/variant.h"
#include "etl/optional.h"
#include "etl/functional.h"

#include "IScreen.hpp"
#include "EinkScreen.hpp"
#include "HolitechScreen.hpp"
#include "DummyScreen.hpp"
#include "DummyScreenController.hpp"

class ScreenFactory
{
public:
    ScreenFactory() = default;
    virtual ~ScreenFactory() = default;

    etl::optional<etl::reference_wrapper<IScreen>> makeScreen(IScreenController& screenController,
                                                              ScreenDefs::ScreenInfo info);

private:
    DummyScreenController mDummyScreenController;
    etl::variant<etl::monostate, DummyScreen, HolitechScreen, EinkScreen> mScreen;
};

#endif /* NEMEIO_DISPLAY_SCREENFACTORY_SCREENFACTORY_HPP_ */
