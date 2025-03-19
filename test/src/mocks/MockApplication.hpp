/*
 * MockApplication.hpp
 *
 *  Created on: Jan 13, 2020
 *      Author: Labo
 */

#ifndef MOCKAPPLICATION_HPP_
#define MOCKAPPLICATION_HPP_

#include "gmock/gmock.h"
#include "IApplication.hpp"

class MockApplication : public IApplication
{
public:
    MOCK_METHOD(bool,
                applyLayoutConfiguration,
                (ILayoutStorage & storage,
                 const configuration::LayoutConfiguration& layoutConfiguration),
                (override));
    MOCK_METHOD(KeyboardParameters, getKeyboardParam, (), (override));
    MOCK_METHOD(void, setKeyboardParam, (const KeyboardParameters& params), (override));
    MOCK_METHOD(void, factoryReset, (), (override));
    MOCK_METHOD(void, FormatFlash, (), (override));
};

#endif /* MOCKAPPLICATION_HPP_ */
