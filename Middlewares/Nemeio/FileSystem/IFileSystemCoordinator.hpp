/*
 * IFileSystemCoordinator.hpp
 *
 *  Created on: Nov 20, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_FILESYSTEM_IFILESYSTEMCOORDINATOR_HPP_
#define NEMEIO_FILESYSTEM_IFILESYSTEMCOORDINATOR_HPP_

#include "IFileSystem.hpp"

namespace fscommon
{
class IFileSystemCoordinator
{
public:
    IFileSystemCoordinator() = default;
    virtual ~IFileSystemCoordinator() = default;
    virtual void notifyFSRequest() = 0;
    virtual void notifyWrite() = 0;
};
}; // namespace fscommon

#endif /* NEMEIO_FILESYSTEM_IFILESYSTEMCOORDINATOR_HPP_ */
