/*
 * AppComm.hpp
 *
 *  Created on: Jan 31, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_APPCOMM_HPP_
#define APPCOMM_APPCOMM_HPP_

#include "AbstractDataTransport.hpp"
#include "AppCommCmd.hpp"
#include <IAppCommTxListener/IAppCommTxListener.hpp>
#include <ICRC32.hpp>
#include "AppCommConfig.h"

#include "FreeRTOS.h"
#include "task.h"

#include "StaticStreamBuffer.hpp"

#include "cmsis_os.h"

#include "IDataTransportRxListener.hpp"
#include <vector>
#include "Mutex.hpp"

class AppComm : public IDataTransportRxListener, public IAppCommTxListener
{
public:
    AppComm(AbstractDataTransport& dataTransport, ICRC32& crc32);
    virtual ~AppComm() = default;

    void init();

    void registerRxListener(IAppCommCmd& listener);

    bool isSameDataSource(const Identifiable& dataSource) const;

    void dataReceived(Identifiable& dataSource, const uint8_t* pData, std::size_t len);

    void sendCmd(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len);
    void sendResponse(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len);

private:
    static constexpr uint8_t sQueueSize = 20;
    static constexpr uint16_t sAppCommTaskPriority = 3;
    static constexpr uint16_t sAppCommTaskStackSize = 1152;

    static constexpr TickType_t DELAY_RECEIVE_FRAME = 5000;

    static constexpr char PROTOCOL_SOF = '\x01';
    static constexpr char PROTOCOL_EOF = '\x04';
    static constexpr uint8_t PROTOCOL_RESP_FLAG = 0x80;


    static constexpr uint16_t PROTOCOL_MIN_FRAME_SIZE = sizeof(PROTOCOL_EOF) + sizeof(uint8_t)
                                                        +                  // CMD
                                                        sizeof(uint16_t) + // LEN
                                                        sizeof(uint32_t) + // CRC32
                                                        sizeof(PROTOCOL_EOF);

    static constexpr uint16_t STREAM_BUFF_SIZE = 4096;
    static constexpr uint16_t STREAM_BUFF_INIT_TRIG = 1;

    struct __attribute__((packed)) FrameHeader
    {
        uint8_t cmdId;
        uint8_t length[sizeof(uint16_t)];
    };

    std::vector<IAppCommCmd*> mCmdListeners;

    AbstractDataTransport& mDataTransport;

    StaticStreamBuffer mRxStreamBuff;
    uint8_t mRxStreamBuffStorage[STREAM_BUFF_SIZE];

    TaskHandle_t mHandle;

    ICRC32& mCRC32;

    Mutex mTxBuffMutex;
    uint8_t mTxBuffer[APP_COMM_PACKET_SIZE];

    uint8_t mRxBuffer[APP_COMM_PACKET_SIZE - sizeof(PROTOCOL_SOF) - sizeof(PROTOCOL_EOF)];
    size_t mRxBufferIndex;

    static void vAppCommTask(void* pvParameters);

    bool receiveFrame();
    bool receiveSOF();
    bool receiveHeader(AppCommDefs::AppCommCmd& cmdId, uint16_t& payloadLength);
    bool receivePayload(size_t length, uint8_t*& pPayload);
    bool receiveCRC32(uint32_t& crc32);
    bool receiveEOF();
    bool receiveDataInRxBuffer(size_t length, uint8_t*& pReceivedData, TickType_t delay);
    bool receiveFromStream(uint8_t* pDataBuff,
                           size_t pDataBuffLen,
                           size_t readLength,
                           TickType_t delay);

    bool transmitToRxListener(AppCommDefs::AppCommCmd cmd, const uint8_t* pPayload, size_t len);

    bool prepareTxFrame(AppCommDefs::AppCommCmd cmd,
                        const uint8_t* pData,
                        size_t len,
                        uint8_t* pTxBuffer,
                        size_t txBufferLen,
                        size_t& totalFrameLength,
                        bool bResponse);
    void sendData(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, size_t len, bool bResponse);
    bool computeCrc32(const uint8_t* buffer, size_t size, uint32_t& crc);
};

#endif /* APPCOMM_APPCOMM_HPP_ */
