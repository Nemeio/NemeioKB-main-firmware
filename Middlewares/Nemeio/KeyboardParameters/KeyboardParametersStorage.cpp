/*
 * KeyboardParametersStorage.cpp
 *
 *  Created on: 9 avr. 2020
 *      Author: eskoric
 */

#include <memory>
#include "KeyboardParametersStorage.hpp"
#include "fs.hpp"
#include "ApplicationConfig.h"
#include "AutoLock.h"
#include "SharedBufferIds.hpp"

KeyboardParametersStorage::KeyboardParametersStorage(
    fs::IFileSystem& fileSystem,
    IKeyboardParametersCodec& keyboardParametersCodec,
    ISharedBufferContainer& sharedBufferContainer)
    : m_fileSystem(fileSystem)
    , m_keyboardParametersCodec(keyboardParametersCodec)
    , m_sharedBufferContainer(sharedBufferContainer)
{
}

void KeyboardParametersStorage::saveKeyboardParameters(const KeyboardParameters& keyboardParameters)
{
    ISharedBuffer& buffer = m_sharedBufferContainer.getSharedBuffer(
        shared_buffer_id::KEYBOARD_PARAMETERS_SHARED_BUFFER_ID);

    AutoLock autoLock(buffer);
    int res;
    fs::File file(m_fileSystem);
    ssize_t size = m_keyboardParametersCodec.encode(keyboardParameters, buffer.buffer());
    bool success = false;

    if(size >= 0)
    {
        success = file.open(PARAMETERS_FILE_NAME,
                            fscommon::OpenFlag::WRONLY | fscommon::OpenFlag::CREAT
                                | fscommon::OpenFlag::TRUNC)
                  == fscommon::FS_ERR_OK;
    }

    if(success)
    {
        file.write(buffer.buffer().data(), size);
        file.close();
    }
}

void KeyboardParametersStorage::readKeyboardParameters(KeyboardParameters& keyboardParameters)
{
    ISharedBuffer& buffer = m_sharedBufferContainer.getSharedBuffer(
        shared_buffer_id::KEYBOARD_PARAMETERS_SHARED_BUFFER_ID);

    AutoLock autoLock(buffer);
    unsigned int byteRead = 0;
    fscommon::File file(m_fileSystem);
    int res;
    std::size_t size;
    KeyboardParameters readKeyboardParameters;
    bool success = false;

    res = file.open(PARAMETERS_FILE_NAME, fscommon::OpenFlag::RDONLY);
    if(res == fscommon::FS_ERR_OK)
    {
        size = file.size();

        if(size <= buffer.buffer().size() && file.read(buffer.buffer().data(), size) == size)
        {
            success = m_keyboardParametersCodec.decode(buffer.buffer(), readKeyboardParameters);
        }

        file.close();
    }

    if(success && readKeyboardParameters.isValid() && readKeyboardParameters != keyboardParameters)
    {
        keyboardParameters = readKeyboardParameters;
    }
}

void KeyboardParametersStorage::deleteKeyboardParameters()
{
    m_fileSystem.remove(PARAMETERS_FILE_NAME);
}
