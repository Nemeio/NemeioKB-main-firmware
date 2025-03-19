/*
 * KeyboardParametersStorage.hpp
 *
 *  Created on: 9 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_KEYBOARDPARAMETERS_KEYBOARDPARAMETERSSTORAGE_HPP_
#define NEMEIO_KEYBOARDPARAMETERS_KEYBOARDPARAMETERSSTORAGE_HPP_

#include "fs.hpp"
#include "IKeyboardParametersStorage.hpp"
#include "IKeyboardParametersCodec.hpp"
#include "ISharedBufferContainer.hpp"

/* TODO Not thread safe for parameters value */
class KeyboardParametersStorage : public IKeyboardParametersStorage
{
public:
    KeyboardParametersStorage(fs::IFileSystem& fileSystem,
                              IKeyboardParametersCodec& keyboardParametersCodec,
                              ISharedBufferContainer& sharedBufferContainer);
    virtual ~KeyboardParametersStorage() = default;

    void saveKeyboardParameters(const KeyboardParameters& keyboardParameters);
    void readKeyboardParameters(KeyboardParameters& keyboardParameters);
    void deleteKeyboardParameters();

private:
    fs::IFileSystem& m_fileSystem;
    IKeyboardParametersCodec& m_keyboardParametersCodec;
    ISharedBufferContainer& m_sharedBufferContainer;
};

#endif /* NEMEIO_KEYBOARDPARAMETERS_KEYBOARDPARAMETERSSTORAGE_HPP_ */
