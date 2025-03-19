/*
 * BrightnessDemoModeHandler.hpp
 *
 *  Created on: 24 janv. 2022
 *      Author: thomas
 */

#ifndef FRONTLIGHT_BRIGHTNESSDEMOMODEHANDLER_HPP_
#define FRONTLIGHT_BRIGHTNESSDEMOMODEHANDLER_HPP_

#include "IDemoListener.hpp"
#include "BrightnessManager.hpp"

class BrightnessDemoModeHandler : public IDemoListener
{
public:
    explicit BrightnessDemoModeHandler(BrightnessManager& brightnessManager);
    virtual ~BrightnessDemoModeHandler() = default;

    void demoStart() final;
    void demoStop() final;

private:
    BrightnessManager& mBrightnessManager;
};

#endif /* FRONTLIGHT_BRIGHTNESSDEMOMODEHANDLER_HPP_ */
