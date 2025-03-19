/*
 * FSNotify.hpp
 *
 *  Created on: Sep 02, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_FILESYSTEM_FSNOTIFY_HPP_
#define NEMEIO_FILESYSTEM_FSNOTIFY_HPP_

class FSNotify
{
public:
    virtual ~FSNotify() = default;
    virtual void doFsAction();
};

#endif // NEMEIO_FILESYSTEM_FSNOTIFY_HPP_
