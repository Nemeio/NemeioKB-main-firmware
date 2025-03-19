#include "ReceiveUserConfigurationCmd.hpp"

ReceiveUserConfigurationCmd::ReceiveUserConfigurationCmd(AppCommDefs::ReceiveDataID id,
                                                         IUserConfigurationStorage& storage)
    : ReceiveDataFileListener(id, storage.getFileSystem(), "")
    , m_storage(storage)
{
}

uint8_t ReceiveUserConfigurationCmd::doPrepareReceive(const uint8_t* pHeader,
                                                      size_t headerLen,
                                                      std::size_t& retDataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(headerLen == uuid::SIZE)
    {
        uuid::UuidView id({reinterpret_cast<const std::byte*>(pHeader), headerLen});
        embedded_ostream path;
        m_storage.getUserConfigurationPath(id, path);

        mPath.assign(path.str().c_str());
        ret = AppCommDefs::APPCOMMERR_SUCCESS;
    }

    if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
    {
        ret = open(retDataLen);
    }

    return ret;
}

void ReceiveUserConfigurationCmd::doReceiveFileDone()
{
    // Nothing to do
}