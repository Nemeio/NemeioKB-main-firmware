#include "ReceiveUuidListCmd.hpp"
#include "UuidListIteratorWrapper.hpp"

ReceiveUuidListCmd::ReceiveUuidListCmd(AppCommDefs::ReceiveDataID id,
                                       IUuidList& list,
                                       UuidListEncoderContainer& encoderContainer)
    : ReceiveDataListener(id)
    , m_list(list)
    , m_encoderContainer(encoderContainer)
{
}

ReceiveUuidListCmd::~ReceiveUuidListCmd()
{
    deleteBuffer();
}

uint8_t ReceiveUuidListCmd::doPrepareReceive(const uint8_t*, size_t, std::size_t& retDataLen)
{
    retDataLen = 0;
    createBuffer();

    if(!m_encoder.has_value() || !m_encoder.value().get().encode(m_list))
    {
        return AppCommDefs::APPCOMMERR_UNEXPECTED;
    }

    m_layoutListDataSize = m_encoder.value().get().getEncodedSize();
    m_currentPos = 0;

    retDataLen = m_layoutListDataSize;

    return AppCommDefs::APPCOMMERR_SUCCESS;
}

uint8_t ReceiveUuidListCmd::doReceiveData(uint8_t* pData, size_t len, std::size_t& retReceiveLength)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;
    retReceiveLength = 0;

    if((m_currentPos + len) > m_layoutListDataSize)
    {
        return AppCommDefs::APPCOMMERR_UNEXPECTED;
    }

    memcpy(pData, m_encoder.value().get().buffer().data() + m_currentPos, len);
    m_currentPos += len;

    retReceiveLength = len;

    return ret;
}

uint8_t ReceiveUuidListCmd::doTerminateReceive()
{
    deleteBuffer();

    return AppCommDefs::APPCOMMERR_SUCCESS;
}

void ReceiveUuidListCmd::doReset()
{
    deleteBuffer();
}

void ReceiveUuidListCmd::createBuffer()
{
    m_encoder = m_encoderContainer.createEncoder();
}

void ReceiveUuidListCmd::deleteBuffer()
{
    if(m_encoder.has_value())
    {
        m_encoderContainer.deleteEncoder(m_encoder.value());
        m_encoder.reset();
    }
}