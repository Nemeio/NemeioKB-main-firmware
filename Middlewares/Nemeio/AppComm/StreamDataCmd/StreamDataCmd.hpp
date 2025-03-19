/*
 * StreamDataCmd.hpp
 *
 *  Created on: Jun 6, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_SENDDATA_STREAMDATACMD_HPP_
#define APPCOMM_SENDDATA_STREAMDATACMD_HPP_

#include <vector>
#include "WriteBuffer.hpp"
#include "AppCommCmd.hpp"
#include "AppCommDefs.hpp"

template<class StreamDataListener, AppCommDefs::AppCommCmd CmdId, typename DataId>
class StreamDataCmd : public AppCommCmd
{
public:
    explicit StreamDataCmd(IAppCommTxListener& appCommTxListener);
    virtual ~StreamDataCmd() = default;

    void dataReceived(const uint8_t* pData, size_t len);

    void registerStreamDataListener(StreamDataListener& listener);

protected:
    static constexpr std::size_t MAX_RESPONSE_SIZE = 132;

private:
    void sendRetCode(uint8_t retCode);
    void nack();

    enum class StreamDataCmdID : uint8_t
    {
        START = 0,
        SEND,
        STOP,
    };

    enum class StreamDataState
    {
        INIT,
        GET_DATA,
    };

    StreamDataListener* mpCurrentStreamDataListener;
    uint32_t mCurrentOffset;
    std::size_t mCurrentStreamDataTotalLength;
    StreamDataState mCurrentState;

    std::vector<StreamDataListener*> mStreamDataListeners;

    WriteBuffer<uint8_t, MAX_RESPONSE_SIZE> mRespBuffer;

    void evtStart(DataId dataId, const uint8_t* pBuff, std::size_t len);
    void evtSend(DataId dataId, const uint8_t* pBuff, std::size_t len);
    void evtStop(DataId dataId, const uint8_t* pBuff, std::size_t len);

    void entryInitState();
    void entryGetDataState();
    void exitInitState();
    void exitGetDataState();

    void resetProtocol();
    uint8_t restartProtocol(DataId dataId, const uint8_t* pBuff, size_t len);

    uint8_t prepare(DataId dataId, const uint8_t* pBuff, std::size_t len);
    uint8_t streamData(DataId dataId, const uint8_t* pBuff, std::size_t len);
    uint8_t terminate(DataId dataId, const uint8_t* pBuff, std::size_t len);

    virtual uint8_t doPrepare(StreamDataListener& listener,
                              const uint8_t* pBuff,
                              std::size_t len,
                              std::size_t& retStreamDataLength,
                              WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff) = 0;
    virtual uint8_t doStreamData(StreamDataListener& listener,
                                 std::size_t remainingStreamLength,
                                 const uint8_t* pBuff,
                                 std::size_t len,
                                 std::size_t& retFrameDataLength,
                                 WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff) = 0;
    virtual uint8_t doTerminate(StreamDataListener& listener,
                                const uint8_t* pBuff,
                                std::size_t len,
                                WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff) = 0;

    void sendResponse(uint8_t retCode, WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff);
};

#include "StreamDataCmd.tpp"

#endif /* APPCOMM_SENDDATA_STREAMDATACMD_HPP_ */
