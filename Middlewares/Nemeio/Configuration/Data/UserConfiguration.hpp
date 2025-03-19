#pragma once

#include "LayoutConfiguration.hpp"

namespace configuration
{
struct LayoutConfigurationDescriptor
{
    uuid::Uuid id;
    uuid::Uuid layoutConfigurationHash;
    uuid::Uuid layoutHash;

    bool operator==(const LayoutConfigurationDescriptor& layoutConfigurationDescriptor) const
    {
        return id == layoutConfigurationDescriptor.id
               && layoutConfigurationHash == layoutConfigurationDescriptor.layoutConfigurationHash
               && layoutHash == layoutConfigurationDescriptor.layoutHash;
    }
};

class UserConfiguration
{
public:
    static const size_t MAX_CONFIGURATIONS = 50;

    explicit UserConfiguration(uuid::UuidView id);
    virtual ~UserConfiguration() = default;

    bool addLayoutConfiguration(const LayoutConfigurationDescriptor& layoutConfigurationDescriptor);

    uuid::UuidView getId() const { return uuid::UuidView(m_id); }
    etl::array_view<const LayoutConfigurationDescriptor> getLayoutConfigurationList() const
    {
        return m_layoutConfigurationList;
    };

    bool operator==(const UserConfiguration& userConfiguration) const
    {
        return m_id == userConfiguration.m_id
               && m_layoutConfigurationList == userConfiguration.m_layoutConfigurationList;
    }

private:
    uuid::Uuid m_id;
    etl::vector<LayoutConfigurationDescriptor, MAX_CONFIGURATIONS> m_layoutConfigurationList{};
};
} // namespace configuration