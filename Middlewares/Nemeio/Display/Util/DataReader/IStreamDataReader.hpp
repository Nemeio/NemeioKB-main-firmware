/*
 * IStreamDataReader.hpp
 *
 *  Created on: Oct 22, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_ISTREAMDATAREADER_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_ISTREAMDATAREADER_HPP_

#include "IDataReader.hpp"

class IStreamDataReader : public IDataReader
{
public:
    IStreamDataReader() = default;
    virtual ~IStreamDataReader() = default;

    virtual bool start() = 0;
    virtual bool runRead() = 0;
    virtual void notifyReadStart() = 0;
    virtual void notifyReadFinish() = 0;
    virtual bool waitReadFinish() = 0;
};

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_ISTREAMDATAREADER_HPP_ */
