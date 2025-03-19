/*
 * ReceiveDataFileListener.hpp
 *
 *  Created on: Jun 10, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_RECEIVEDATA_RECEIVEDATALISTENERS_RECEIVEDATAFILELISTENER_HPP_
#define APPCOMM_RECEIVEDATA_RECEIVEDATALISTENERS_RECEIVEDATAFILELISTENER_HPP_

#include "FileIStream.hpp"
#include "ReceiveDataListener.hpp"
#include "etl/string.h"

class ReceiveDataFileListener : public ReceiveDataListener
{
public:
    ReceiveDataFileListener(AppCommDefs::ReceiveDataID dataId,
                            fs::IFileSystem& fileSystem,
                            std::string_view path);
    virtual ~ReceiveDataFileListener() = default;

protected:
    etl::string<fs::MAX_PATH_SIZE> mPath;
    fs::IFileSystem& mFileSystem;

    uint8_t open(std::size_t& retDataLen);

private:
    FileIStream mFileStream;

    uint8_t doPrepareReceive(const uint8_t* pHeader,
                             size_t headerLen,
                             std::size_t& retDataLen) override;
    uint8_t doReceiveData(uint8_t* pData, size_t len, std::size_t& retReceiveLength) final;
    uint8_t doTerminateReceive() override;
    void doReset() final;

    virtual void doReceiveFileDone();
};

#endif /* APPCOMM_RECEIVEDATA_RECEIVEDATALISTENERS_RECEIVEDATAFILELISTENER_HPP_ */
