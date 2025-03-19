#pragma once

#include "IUserConfigurationNavigation.hpp"
#include "ConfigurationStorageContext.hpp"
#include "IApplication.hpp"
#include "IConfigurationChangeNotifier.hpp"

class UserConfigurationNavigation : public IUserConfigurationNavigation
{
public:
    UserConfigurationNavigation(uuid::UuidView id,
                                ConfigurationStorageContext& storageContext,
                                IApplication& application,
                                IConfigurationChangeNotifier& confChangeNotifier);
    ~UserConfigurationNavigation() final = default;

    configuration::Ret enableCustom() final;
    configuration::Ret disableCustom() final;
    configuration::Ret applyFirst() final;
    configuration::Ret applyNext(SwitchDirection direction) final;
    configuration::Ret applyId(uuid::UuidView id) final;

    etl::optional<uuid::UuidView> getCurrentUserConfigurationId() const final
    {
        etl::optional<uuid::UuidView> ret{etl::nullopt};

        if(m_userConfiguration.has_value())
        {
            ret.emplace(m_userConfiguration.value().getId());
        }

        return ret;
    }

private:
    using ConstIterator =
        etl::vector<configuration::LayoutConfigurationDescriptor,
                    configuration::UserConfiguration::MAX_CONFIGURATIONS>::const_iterator;

    ConfigurationStorageContext& m_storageContext;
    etl::optional<configuration::UserConfiguration> m_userConfiguration;
    etl::optional<configuration::LayoutConfiguration> m_currentLayoutConfiguration;
    etl::optional<configuration::LayoutConfiguration> m_tempLayoutConfiguration;
    IApplication& m_application;
    IConfigurationChangeNotifier& m_confChangeNotifier;

    ConstIterator m_layoutConfigurationIt{nullptr};

    bool m_enableCustom{false};

    ConstIterator iterate(
        const etl::array_view<const configuration::LayoutConfigurationDescriptor>& list,
        const ConstIterator& it,
        SwitchDirection direction) const;
    configuration::Ret applyIterator(
        const etl::array_view<const configuration::LayoutConfigurationDescriptor>& list,
        ConstIterator it);
    configuration::Ret applyLayoutConfiguration(
        const configuration::LayoutConfiguration& layoutConfiguration);
    ConstIterator findLayoutConfigurationId(
        uuid::UuidView id,
        const etl::array_view<const configuration::LayoutConfigurationDescriptor>& list) const;
};