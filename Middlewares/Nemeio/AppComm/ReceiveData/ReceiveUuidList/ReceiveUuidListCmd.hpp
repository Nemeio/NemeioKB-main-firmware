#pragma once

#include "ReceiveDataListener.hpp"
#include "UuidListEncoderContainer.hpp"
#include "etl/vector.h"
#include "cpp_helper.hpp"

class ReceiveUuidListCmd : public ReceiveDataListener
{
public:
    explicit ReceiveUuidListCmd(AppCommDefs::ReceiveDataID id,
                                IUuidList& list,
                                UuidListEncoderContainer& encoderContainer);
    ~ReceiveUuidListCmd() final;
    COPYABLE_MOVABLE(ReceiveUuidListCmd, delete);

private:
    IUuidList& m_list;
    UuidListEncoderContainer& m_encoderContainer;
    etl::optional<etl::reference_wrapper<UuidListEncoder>> m_encoder;
    ssize_t m_layoutListDataSize{0};
    size_t m_currentPos{0};

    uint8_t doPrepareReceive(const uint8_t* pHeader,
                             size_t headerLen,
                             std::size_t& retDataLen) final;
    uint8_t doReceiveData(uint8_t* pData, size_t len, std::size_t& retReceiveLength) final;
    uint8_t doTerminateReceive() final;
    void doReset() final;
    void createBuffer();
    void deleteBuffer();
};