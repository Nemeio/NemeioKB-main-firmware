#include "DeleteUserConfigurationCmd.hpp"
#include "AppCommUtils.hpp"

DeleteUserConfigurationCmd::DeleteUserConfigurationCmd(AppCommDefs::AppCommCmd id,
                                                       IAppCommTxListener& appCommTxListener,
                                                       IUserConfigurationStorage& storage)
    : AppCommCmd(id, appCommTxListener)
    , m_storage(storage)
{
}

void DeleteUserConfigurationCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    if(dataLen != uuid::SIZE)
    {
        ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;
    }

    if(ret == AppCommDefs::APPCOMMERR_SUCCESS)
    {
        uuid::UuidView id(
            etl::array_view<const std::byte>{reinterpret_cast<const std::byte*>(pData), dataLen});
        configuration::Ret storageRet = m_storage.deleteUserConfiguration(id);
        ret = AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(storageRet);
    }

    transmitResponse(&ret, sizeof(ret));
}