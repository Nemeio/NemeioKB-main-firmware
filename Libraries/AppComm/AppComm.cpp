/*
 * AppComm.cpp
 *
 *  Created on: Jan 31, 2019
 *      Author: Labo
 */
#include "AppComm.hpp"
#include <string.h>
#include "assertTools.h"
#include "AppCommUtils.hpp"
#include "AppCommConfig.h"
#include "AutoLock.h"

#define TEST_FWUPDATE
#define TEST_FWUPDATE_ECHO

AppComm::AppComm(AbstractDataTransport& dataTransport, ICRC32& crc32)
    : mDataTransport(dataTransport)
    , mRxStreamBuff(STREAM_BUFF_SIZE, STREAM_BUFF_INIT_TRIG, mRxStreamBuffStorage)
    , mCRC32(crc32)
    , mRxBufferIndex(0)
{
    mHandle = NULL;

    BaseType_t ret = xTaskCreate(vAppCommTask,
                                 "AppCommTask",
                                 sAppCommTaskStackSize,
                                 this,
                                 sAppCommTaskPriority,
                                 &mHandle);
    ASSERT(ret == pdPASS);
}

void AppComm::init()
{
}

void AppComm::dataReceived(Identifiable& dataSource, const uint8_t* pData, std::size_t len)
{
    ASSERT(dataSource == mDataTransport);

    size_t sentBytes = mRxStreamBuff.send(pData, len, portMAX_DELAY);
    ASSERT(sentBytes == len);
}

void AppComm::registerRxListener(IAppCommCmd& listener)
{
    mCmdListeners.push_back(&listener);
}

void AppComm::vAppCommTask(void* pvParameters)
{
    AppComm* appComm = (AppComm*) pvParameters;

    ASSERT(appComm != NULL);

    for(;;)
    {
        appComm->receiveFrame();
    }
}

bool AppComm::transmitToRxListener(AppCommDefs::AppCommCmd cmd, const uint8_t* pPayload, size_t len)
{
    bool bRet = false;

    for(auto ite = mCmdListeners.begin(); ite != mCmdListeners.end(); ++ite)
    {
        IAppCommCmd* pListener = *ite;
        if(pListener != NULL && pListener->getCmdId() == cmd)
        {
            pListener->dataReceived(pPayload, len);
            bRet = true;
            break;
        }
    }

    return bRet;
}

bool AppComm::prepareTxFrame(AppCommDefs::AppCommCmd cmd,
                             const uint8_t* pData,
                             size_t len,
                             uint8_t* pTxBuffer,
                             size_t txBufferLen,
                             size_t& totalFrameLength,
                             bool bResponse)
{
    bool bRet = false;

    ASSERT(pTxBuffer != NULL);
    ASSERT(len == 0 || pData != NULL);

    totalFrameLength = PROTOCOL_MIN_FRAME_SIZE + len;

    if(totalFrameLength <= txBufferLen)
    {
        memset(pTxBuffer, 0, txBufferLen);

        size_t frameIdx = 0;
        uint8_t* pCRC32Buf = NULL;
        size_t CRC32BufLen = 0;
        uint8_t cmdValue = static_cast<uint8_t>(cmd);

        if(bResponse)
        {
            cmdValue |= PROTOCOL_RESP_FLAG;
        }

        AppCommUtils::writeU8(PROTOCOL_SOF, pTxBuffer, txBufferLen, frameIdx, frameIdx);

        // CRC32 buffer starts after SOF
        pCRC32Buf = &pTxBuffer[frameIdx];

        AppCommUtils::writeU8(cmdValue, pTxBuffer, txBufferLen, frameIdx, frameIdx);

        AppCommUtils::writeU16(len, pTxBuffer, txBufferLen, frameIdx, frameIdx);

        if(pData != NULL)
        {
            memcpy(&pTxBuffer[frameIdx], pData, len);
        }

        frameIdx += len;

        // CRC32 buffer ends after payload
        CRC32BufLen = frameIdx - sizeof(PROTOCOL_SOF);

        if(uint32_t crc32 = 0; computeCrc32(pCRC32Buf, CRC32BufLen, crc32))
        {
            AppCommUtils::writeU32(crc32, pTxBuffer, txBufferLen, frameIdx, frameIdx);
        }

        AppCommUtils::writeU8(PROTOCOL_EOF, pTxBuffer, txBufferLen, frameIdx, frameIdx);

        bRet = true;
    }

    return bRet;
}

void AppComm::sendData(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, size_t len, bool bResponse)
{
    AutoLock autoLock(mTxBuffMutex);

    size_t totalFrameLength = 0;

    if(prepareTxFrame(cmd, pData, len, mTxBuffer, sizeof(mTxBuffer), totalFrameLength, bResponse))
    {
        mDataTransport.sendData(mTxBuffer, totalFrameLength);
    }
    else
    {
        ASSERT(false);
    }
}

void AppComm::sendCmd(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len)
{
    sendData(cmd, pData, len, false);
}

bool AppComm::receiveFrame()
{
    bool bRet = false;
    AppCommDefs::AppCommCmd cmdId = AppCommDefs::AppCommCmd::SENDDATA;
    uint16_t payloadLength = 0;
    uint32_t crc32 = 0;
    uint8_t* pPayload = NULL;
    uint32_t computedCRC32 = 0;

    mRxBufferIndex = 0;

    bRet = receiveSOF() && receiveHeader(cmdId, payloadLength)
           && receivePayload(payloadLength, pPayload) && receiveCRC32(crc32) && receiveEOF()
           && computeCrc32(mRxBuffer, mRxBufferIndex, computedCRC32) && (computedCRC32 == crc32)
           && transmitToRxListener(cmdId, pPayload, payloadLength);

    return bRet;
}

bool AppComm::receiveSOF()
{
    bool bRet = false;
    char sof = '\0';

    bRet = receiveFromStream(reinterpret_cast<uint8_t*>(&sof),
                             sizeof(sof),
                             sizeof(PROTOCOL_SOF),
                             portMAX_DELAY);
    ASSERT(bRet);

    return PROTOCOL_SOF == sof;
}

bool AppComm::receiveHeader(AppCommDefs::AppCommCmd& cmdId, uint16_t& payloadLength)
{
    bool bRet = false;
    uint8_t* frameHeaderBuff = NULL;

    bRet = receiveDataInRxBuffer(sizeof(FrameHeader), frameHeaderBuff, DELAY_RECEIVE_FRAME);
    if(bRet && frameHeaderBuff != NULL)
    {
        size_t dummyIdx = 0;
        FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(frameHeaderBuff);

        cmdId = static_cast<AppCommDefs::AppCommCmd>(pFrameHeader->cmdId);
        AppCommUtils::parseU16(pFrameHeader->length,
                               sizeof(pFrameHeader->length),
                               0,
                               payloadLength,
                               dummyIdx);
    }

    if(sizeof(FrameHeader) + payloadLength > sizeof(mRxBuffer))
    {
        return false;
    }

    return bRet;
}

bool AppComm::receivePayload(size_t length, uint8_t*& pPayload)
{
    bool bRet = false;

    bRet = receiveDataInRxBuffer(length, pPayload, DELAY_RECEIVE_FRAME);

    return bRet;
}

bool AppComm::receiveCRC32(uint32_t& crc32)
{
    bool bRet = false;
    uint8_t CRC32Buff[sizeof(uint32_t)] = {0};

    bRet = receiveFromStream(reinterpret_cast<uint8_t*>(&CRC32Buff),
                             sizeof(CRC32Buff),
                             sizeof(CRC32Buff),
                             DELAY_RECEIVE_FRAME);

    if(bRet)
    {
        size_t dummyIdx = 0;
        AppCommUtils::parseU32(CRC32Buff, sizeof(CRC32Buff), 0, crc32, dummyIdx);
    }

    return bRet;
}

bool AppComm::receiveEOF()
{
    bool bRet = false;
    char eof = '\0';

    if(receiveFromStream(reinterpret_cast<uint8_t*>(&eof),
                         sizeof(eof),
                         sizeof(PROTOCOL_EOF),
                         DELAY_RECEIVE_FRAME)
       && (PROTOCOL_EOF == eof))
    {
        bRet = true;
    }

    return bRet;
}

bool AppComm::receiveDataInRxBuffer(size_t length, uint8_t*& pReceivedData, TickType_t delay)
{
    ASSERT(mRxBufferIndex <= sizeof(mRxBuffer));

    pReceivedData = mRxBuffer + mRxBufferIndex;

    bool bRet = receiveFromStream(mRxBuffer + mRxBufferIndex,
                                  sizeof(mRxBuffer) - mRxBufferIndex,
                                  length,
                                  delay);

    if(bRet)
    {
        mRxBufferIndex += length;
    }

    return bRet;
}

bool AppComm::receiveFromStream(uint8_t* pDataBuff,
                                size_t pDataBuffLen,
                                size_t readLength,
                                TickType_t delay)
{
    ASSERT(pDataBuff != NULL);
    ASSERT(readLength <= pDataBuffLen);

    size_t receivedBytes = mRxStreamBuff.receiveNBytes(pDataBuff, pDataBuffLen, readLength, delay);

    return readLength == receivedBytes;
}

bool AppComm::isSameDataSource(const Identifiable& dataSource) const
{
    return dataSource == mDataTransport;
}

void AppComm::sendResponse(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len)
{
    sendData(cmd, pData, len, true);
}

bool AppComm::computeCrc32(const uint8_t* buffer, size_t size, uint32_t& crc)
{
    AutoLock crcLock(mCRC32);

    mCRC32.configure();

    return mCRC32.compute(buffer, size, crc);
}