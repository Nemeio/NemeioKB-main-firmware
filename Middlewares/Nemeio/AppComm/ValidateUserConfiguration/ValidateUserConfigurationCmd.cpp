#include "ValidateUserConfigurationCmd.hpp"
#include "AppCommUtils.hpp"
#include "ConfigurationStorageUtil.hpp"
#include "UuidListIteratorWrapper.hpp"

ValidateUserConfigurationCmd::ValidateUserConfigurationCmd(
    AppCommDefs::AppCommCmd id,
    IAppCommTxListener& appCommTxListener,
    IUserConfigurationWritersContainer& userConfigurationWritersContainer,
    etl::string_view validDirectoryPath,
    IUserConfigurationStorage& storage)
    : AppCommCmd(id, appCommTxListener)
    , m_userConfigurationWritersContainer(userConfigurationWritersContainer)
    , m_validDirectoryPath(validDirectoryPath)
    , m_storage(storage)
{
}

void ValidateUserConfigurationCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if((uuid::SIZE == dataLen) && (pData != nullptr))
    {
        retCode = AppCommDefs::APPCOMMERR_UNEXPECTED;

        uuid::UuidView id(
            etl::array_view<const std::byte>{reinterpret_cast<const std::byte*>(pData), dataLen});

        auto writer = m_userConfigurationWritersContainer.getUserConfigurationWriter(id);

        if(writer.has_value())
        {
            embedded_ostream validPath;
            validPath << m_validDirectoryPath << '/';
            validPath << writer.value().get().getId();

            configuration::Ret confRet = writer.value().get().move(validPath.str().c_str());
            retCode = AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(confRet);

            notify_observers(UserConfigurationUpdateEvent{writer.value().get().getId()});

            cleanupUserConfigurations(writer.value().get().getId());
        }
    }


    replyRetCode(retCode);
}

void ValidateUserConfigurationCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}

void ValidateUserConfigurationCmd::cleanupUserConfigurations(uuid::UuidView idToKeep)
{
    UuidListIteratorWrapper itWrapper(m_storage);
    if(!itWrapper.it().has_value())
    {
        return;
    }

    auto& it = itWrapper.it().value().get();
    auto id = it.next();
    while(id.has_value())
    {
        if(id.value() != idToKeep)
        {
            m_storage.deleteUserConfiguration(uuid::UuidView{id.value()});
        }

        id = it.next();
    }
}