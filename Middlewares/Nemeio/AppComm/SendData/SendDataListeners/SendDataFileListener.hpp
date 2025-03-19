/*
 * SendDataFileListener.hpp
 *
 *  Created on: Feb 5, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_SENDDATA_SENDDATAFILELISTENER_HPP_
#define APPCOMM_SENDDATA_SENDDATAFILELISTENER_HPP_

#include "FileIOStream.hpp"
#include "SendDataListener.hpp"
#include <string>
#include "LittleFileSystem.hpp"

class SendDataFileListener : public SendDataListener
{
public:
    SendDataFileListener(AppCommDefs::SendDataID dataId,
                         fs::IFileSystem& fileSystem,
                         std::string_view path);
    virtual ~SendDataFileListener();
    SendDataFileListener(SendDataFileListener&) = delete;
    SendDataFileListener& operator=(const SendDataFileListener&) = delete;
    SendDataFileListener(SendDataFileListener&&) = delete;
    SendDataFileListener const& operator=(SendDataFileListener&&) = delete;

protected:
    uint8_t open();

    [[nodiscard]] fs::IFileSystem& getFileSystem();
    [[nodiscard]] std::string_view getPath() const;
    [[nodiscard]] FileIStream& getFileStreamRead();


private:
    std::string_view mPath;
    FileIOStream mFileStream;
    bool mbFileOpen = false;
    fs::IFileSystem& mFileSystem;

    uint8_t write(const uint8_t* pData, size_t len);
    uint8_t close();
    void closeAndDelete();

    uint8_t doPrepareSend(const uint8_t* pHeader, size_t headerLen, size_t dataLen) override;
    uint8_t doSendData(const uint8_t* pData, size_t len);
    uint8_t doTerminateSend();
    void doReset();
};

#endif /* APPCOMM_SENDDATA_SENDDATAFILELISTENER_HPP_ */
