#include "UserConfiguration.hpp"

using namespace configuration;

UserConfiguration::UserConfiguration(uuid::UuidView id)
    : m_id(id)
{
}

bool UserConfiguration::addLayoutConfiguration(
    const LayoutConfigurationDescriptor& layoutConfigurationDescriptor)
{
    bool ret = false;

    if(!m_layoutConfigurationList.full())
    {
        m_layoutConfigurationList.push_back(layoutConfigurationDescriptor);
        ret = true;
    }

    return ret;
}