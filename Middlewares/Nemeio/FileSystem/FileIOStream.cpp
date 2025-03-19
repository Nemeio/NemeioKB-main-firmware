/*
 * FileIOStream.cpp
 *
 *  Created on: Jan 7, 2021
 *      Author: eskoric
 */

#include "FileIOStream.hpp"

FileIOStream::FileIOStream(fs::IFileSystem& fileSystem)
    : FileStream(fileSystem)
    , FileIStream(fileSystem)
    , FileOStream(fileSystem)
{
}
