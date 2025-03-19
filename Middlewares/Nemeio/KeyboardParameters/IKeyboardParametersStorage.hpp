/*
 * IKeyboardParametersStorage.hpp
 *
 *  Created on: 9 avr. 2020
 *      Author: eskoric
 */

#ifndef KEYBOARDPARAMETERS_IKEYBOARDPARAMETERSSTORAGE_HPP_
#define KEYBOARDPARAMETERS_IKEYBOARDPARAMETERSSTORAGE_HPP_

#include "KeyboardParameters.hpp"

class IKeyboardParametersStorage
{
public:
    IKeyboardParametersStorage() = default;
    virtual ~IKeyboardParametersStorage() = default;

    virtual void saveKeyboardParameters(const KeyboardParameters& keyboardParameters) = 0;
    virtual void readKeyboardParameters(KeyboardParameters& keyboardParameters) = 0;
    virtual void deleteKeyboardParameters() = 0;
};

#endif /* KEYBOARDPARAMETERS_IKEYBOARDPARAMETERSSTORAGE_HPP_ */
