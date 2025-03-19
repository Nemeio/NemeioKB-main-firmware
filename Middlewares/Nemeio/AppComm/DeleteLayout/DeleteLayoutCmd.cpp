#include "DeleteLayoutCmd.hpp"
#include "AppCommUtils.hpp"

DeleteLayoutCmd::DeleteLayoutCmd(AppCommDefs::AppCommCmd id,
                                 IAppCommTxListener& appCommTxListener,
                                 ILayoutStorage& storage)
    : AppCommCmd(id, appCommTxListener)
    , m_storage(storage)
{
}

void DeleteLayoutCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    if(dataLen != uuid::SIZE)
    {
        ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;
    }

    if(ret == AppCommDefs::APPCOMMERR_SUCCESS)
    {
        uuid::UuidView hash(
            etl::array_view<const std::byte>{reinterpret_cast<const std::byte*>(pData), dataLen});
        configuration::Ret storageRet = m_storage.deleteLayout(hash);
        ret = AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(storageRet);
    }

    transmitResponse(&ret, sizeof(ret));
}