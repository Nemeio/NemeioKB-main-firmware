/*
 * StreamDataListener.hpp
 *
 *  Created on: Jun 10, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_APPCOMM_STREAMDATACMD_STREAMDATALISTENER_HPP_
#define NEMEIO_APPCOMM_STREAMDATACMD_STREAMDATALISTENER_HPP_

#include "AppCommDefs.hpp"

template<typename DataId>
class StreamDataListener
{
public:
    explicit StreamDataListener(DataId dataId);
    virtual ~StreamDataListener() = default;

    DataId getDataId() const;
    void reset();

private:
    const DataId mDataId;

    virtual void doReset() = 0;
};

template<typename DataId>
inline StreamDataListener<DataId>::StreamDataListener(DataId dataId)
    : mDataId(dataId)
{
}

template<typename DataId>
inline DataId StreamDataListener<DataId>::getDataId() const
{
    return mDataId;
}

template<typename DataId>
inline void StreamDataListener<DataId>::reset()
{
    doReset();
}

#endif /* NEMEIO_APPCOMM_STREAMDATACMD_STREAMDATALISTENER_HPP_ */
