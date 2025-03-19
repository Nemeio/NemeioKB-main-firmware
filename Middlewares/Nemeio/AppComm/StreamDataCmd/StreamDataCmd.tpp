/*
 * StreamDataCmd.tpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Labo
 */

#include "assertTools.h"
#include "StreamDataCmd.hpp"
#include "AppCommUtils.hpp"

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
StreamDataCmd<StreamDataListener, CmdId, DataId>::StreamDataCmd(IAppCommTxListener& appCommTxListener)
    : AppCommCmd(CmdId, appCommTxListener)
    , mpCurrentStreamDataListener(nullptr)
    , mCurrentOffset(0)
    , mCurrentStreamDataTotalLength(0)
    , mCurrentState(StreamDataState::INIT)
{
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::dataReceived(const uint8_t* pData, size_t len)
{
    ASSERT(pData != NULL);

    if(len < (sizeof(DataId) + sizeof(StreamDataCmdID)))
    {
        return;
    }

    size_t idx = 0;

    DataId dataId = static_cast<DataId>(pData[idx++]);
    --len;

    StreamDataCmdID cmdID = static_cast<StreamDataCmdID>(pData[idx++]);
    --len;

    switch(cmdID)
    {
    case StreamDataCmdID::START:
        evtStart(dataId, &pData[idx], len);
        break;
    case StreamDataCmdID::SEND:
        evtSend(dataId, &pData[idx], len);
        break;
    case StreamDataCmdID::STOP:
        evtStop(dataId, &pData[idx], len);
        break;
    default:
        nack();
        break;
    }
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::evtStart(DataId dataId,
                                                                const uint8_t* pBuff,
                                                                std::size_t len)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    switch(mCurrentState)
    {
    case StreamDataState::INIT:
        ret = prepare(dataId, pBuff, len);

        if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
        {
            exitInitState();
            mCurrentState = StreamDataState::GET_DATA;
            entryGetDataState();
        }
        else
        {
            exitInitState();
            mCurrentState = StreamDataState::INIT;
            entryInitState();
        }
        break;
    case StreamDataState::GET_DATA:
        ret = restartProtocol(dataId, pBuff, len);

        if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
        {
            // Stay in GetData state
        }
        else
        {
            exitGetDataState();
            mCurrentState = StreamDataState::INIT;
            entryInitState();
        }
        break;
    default:
        ASSERT(false);
        break;
    }
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::evtSend(DataId dataId,
                                                               const uint8_t* pBuff,
                                                               std::size_t len)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    switch(mCurrentState)
    {
    case StreamDataState::INIT:
        nack();
        // Stay in Init state
        break;
    case StreamDataState::GET_DATA:
        ret = streamData(dataId, pBuff, len);

        if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
        {
            // Stay in GetData state
        }
        else
        {
            exitGetDataState();
            mCurrentState = StreamDataState::INIT;
            entryInitState();
        }
        break;
    default:
        ASSERT(false);
        break;
    }
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::evtStop(DataId dataId,
                                                               const uint8_t* pBuff,
                                                               std::size_t len)
{
    switch(mCurrentState)
    {
    case StreamDataState::INIT:
        nack();
        // Stay in Init state
        break;
    case StreamDataState::GET_DATA:
        terminate(dataId, pBuff, len);

        exitGetDataState();
        mCurrentState = StreamDataState::INIT;
        entryInitState();
        break;
    default:
        ASSERT(false);
        break;
    }
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::nack()
{
    sendRetCode(AppCommDefs::APPCOMMERR_STREAMDATA_PROTOCOL);
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::sendRetCode(uint8_t retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::registerStreamDataListener(
    StreamDataListener& listener)
{
    mStreamDataListeners.push_back(&listener);
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::entryInitState()
{
    resetProtocol();
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::entryGetDataState()
{
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::exitInitState()
{
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::exitGetDataState()
{
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
void StreamDataCmd<StreamDataListener, CmdId, DataId>::resetProtocol()
{
    if(mpCurrentStreamDataListener != NULL)
    {
        mpCurrentStreamDataListener->reset();
    }
    mpCurrentStreamDataListener = NULL;
    mCurrentOffset = 0;
    mCurrentStreamDataTotalLength = 0;
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
uint8_t StreamDataCmd<StreamDataListener, CmdId, DataId>::restartProtocol(DataId dataId,
                                                                          const uint8_t* pBuff,
                                                                          size_t len)
{
    resetProtocol();
    return prepare(dataId, pBuff, len);
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
inline uint8_t StreamDataCmd<StreamDataListener, CmdId, DataId>::prepare(DataId dataId,
                                                                         const uint8_t* pBuff,
                                                                         std::size_t len)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_STREAMDATA_PROTOCOL;

    ASSERT(pBuff != nullptr);
    ASSERT(nullptr == mpCurrentStreamDataListener);

    for(auto ite = mStreamDataListeners.begin(); ite != mStreamDataListeners.end(); ++ite)
    {
        StreamDataListener* pListener = *ite;
        if(pListener != NULL && pListener->getDataId() == dataId)
        {
            mpCurrentStreamDataListener = pListener;
            break;
        }
    }

    if(mpCurrentStreamDataListener != nullptr)
    {
        mRespBuffer.reset();
        ret = doPrepare(*mpCurrentStreamDataListener,
                        pBuff,
                        len,
                        mCurrentStreamDataTotalLength,
                        mRespBuffer);
    }

    sendResponse(ret, mRespBuffer);

    return ret;
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
inline uint8_t StreamDataCmd<StreamDataListener, CmdId, DataId>::streamData(DataId dataId,
                                                                            const uint8_t* pBuff,
                                                                            std::size_t len)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_STREAMDATA_PROTOCOL;
    uint32_t frameOffset = 0;

    ASSERT(pBuff != NULL);

    if(mpCurrentStreamDataListener != nullptr && mpCurrentStreamDataListener->getDataId() == dataId
       && len > sizeof(frameOffset))
    {
        size_t idx = 0;

        AppCommUtils::parseU32(pBuff, len, idx, frameOffset, idx);

        pBuff += idx;
        len -= sizeof(frameOffset);

        if((frameOffset == mCurrentOffset))
        {
            const std::size_t remainingStreamLength = mCurrentStreamDataTotalLength
                                                      - mCurrentOffset;
            std::size_t retFrameDataLength = 0;

            mRespBuffer.reset();
            ret = doStreamData(*mpCurrentStreamDataListener,
                               remainingStreamLength,
                               pBuff,
                               len,
                               retFrameDataLength,
                               mRespBuffer);

            if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
            {
                mCurrentOffset += retFrameDataLength;
            }
        }
    }

    sendResponse(ret, mRespBuffer);

    return ret;
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
inline uint8_t StreamDataCmd<StreamDataListener, CmdId, DataId>::terminate(DataId dataId,
                                                                           const uint8_t* pBuff,
                                                                           std::size_t len)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_STREAMDATA_PROTOCOL;

    if(mCurrentOffset == mCurrentStreamDataTotalLength)
    {
        if(mpCurrentStreamDataListener != nullptr
           && mpCurrentStreamDataListener->getDataId() == dataId)
        {
            mRespBuffer.reset();
            ret = doTerminate(*mpCurrentStreamDataListener, pBuff, len, mRespBuffer);
        }
    }

    sendResponse(ret, mRespBuffer);

    return ret;
}

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
inline void StreamDataCmd<StreamDataListener, CmdId, DataId>::sendResponse(
    uint8_t retCode, WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff)
{
    if(AppCommDefs::APPCOMMERR_SUCCESS == retCode)
    {
        transmitResponse(mRespBuffer.data(), mRespBuffer.usedSpace());
    }
    else
    {
        sendRetCode(retCode);
    }
}
