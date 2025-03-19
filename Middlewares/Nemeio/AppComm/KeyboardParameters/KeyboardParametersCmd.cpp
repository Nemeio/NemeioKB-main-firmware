/*
 * KeyboardParametersCmd.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: Labo
 */

#include "KeyboardParametersCmd.hpp"
#include "utils.hpp"
#include "SharedBufferIds.hpp"

KeyboardParametersCmd::KeyboardParametersCmd(IAppCommTxListener& appCommTxListener,
                                             IApplication& application,
                                             IKeyboardParametersCodec& codec,
                                             ISharedBufferContainer& sharedBufferContainer)
    : AppCommCmd(AppCommDefs::AppCommCmd::KEYBOARDPARAMETERS, appCommTxListener)
    , m_application(application)
    , m_codec(codec)
    , m_sharedBufferContainer(sharedBufferContainer)
{
}

void KeyboardParametersCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    KeyboardParametersCmdId cmdId = KeyboardParametersCmdId::UNDEFINED;
    AppCommDefs::AppCommErrCode ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(dataLen >= sizeof(KeyboardParametersCmdId))
    {
        cmdId = static_cast<KeyboardParametersCmdId>(pData[0]);
        ++pData;
        --dataLen;

        etl::array_view<const std::byte> payload{reinterpret_cast<const std::byte*>(pData), dataLen};
        switch(cmdId)
        {
        case KeyboardParametersCmdId::GET:
            getParameters(payload);
            break;
        case KeyboardParametersCmdId::SET:
            setParameters(payload);
            break;
        default:
            sendRetCode(cmdId, ret);
            break;
        }
    }
    else
    {
        sendRetCode(cmdId, ret);
    }
}

void KeyboardParametersCmd::getParameters(etl::array_view<const std::byte> data)
{
    if(data.empty())
    {
        KeyboardParameters parameters = m_application.getKeyboardParam();
        sendParameters(parameters);
    }
    else
    {
        sendRetCode(KeyboardParametersCmdId::GET, AppCommDefs::APPCOMMERR_MALFORMED_DATA);
    }
}

void KeyboardParametersCmd::setParameters(etl::array_view<const std::byte> data)
{
    AppCommDefs::AppCommErrCode ret = AppCommDefs::APPCOMMERR_INVALID_CONTENT;

    KeyboardParameters parameters = m_application.getKeyboardParam();

    if(m_codec.decode(data, parameters) && parameters.isValid())
    {
        m_application.setKeyboardParam(parameters);
        ret = AppCommDefs::APPCOMMERR_SUCCESS;
    }

    sendRetCode(KeyboardParametersCmdId::SET, ret);
}

void KeyboardParametersCmd::sendRetCode(KeyboardParametersCmdId cmdId,
                                        AppCommDefs::AppCommErrCode ret)
{
    KeyboardParametersResponse response(cmdId, ret);
    transmitResponse(reinterpret_cast<const uint8_t*>(&response), sizeof(response));
}

void KeyboardParametersCmd::sendParameters(const KeyboardParameters& parameters)
{
    ISharedBuffer& responseBuffer = m_sharedBufferContainer.getSharedBuffer(
        shared_buffer_id::KEYBOARD_PARAMETERS_SHARED_BUFFER_ID);
    AutoLock autoLock(responseBuffer);
    size_t responseSize = 0;
    size_t remainingSize = responseBuffer.buffer().size();

    if(remainingSize < sizeof(KeyboardParametersResponse))
    {
        sendRetCode(KeyboardParametersCmdId::SET, AppCommDefs::APPCOMMERR_UNEXPECTED);
        return;
    }

    auto& response = *reinterpret_cast<KeyboardParametersResponse*>(responseBuffer.buffer().data());
    response.cmdId = KeyboardParametersCmdId::GET;
    response.ret = AppCommDefs::APPCOMMERR_UNEXPECTED;

    responseSize += sizeof(KeyboardParametersResponse);
    remainingSize -= sizeof(KeyboardParametersResponse);

    etl::array_view<std::byte> encodeBuffer{reinterpret_cast<std::byte*>(response.payload),
                                            remainingSize};

    if(ssize_t encodeSize = m_codec.encode(parameters, encodeBuffer); encodeSize >= 0)
    {
        responseSize += encodeSize;
        response.ret = AppCommDefs::APPCOMMERR_SUCCESS;
    }

    transmitResponse(reinterpret_cast<const uint8_t*>(responseBuffer.buffer().data()), responseSize);
}