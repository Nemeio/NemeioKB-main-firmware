#include "UserConfigurationNavigation.hpp"

using namespace configuration;

UserConfigurationNavigation::UserConfigurationNavigation::UserConfigurationNavigation(
    uuid::UuidView id,
    ConfigurationStorageContext& storageContext,
    IApplication& application,
    IConfigurationChangeNotifier& confChangeNotifier)
    : m_storageContext(storageContext)
    , m_application(application)
    , m_confChangeNotifier(confChangeNotifier)
{
    m_storageContext.getUserConfigurationStorage().getUserConfiguration(id, m_userConfiguration);

    if(m_userConfiguration.has_value())
    {
        m_layoutConfigurationIt = m_userConfiguration.value().getLayoutConfigurationList().begin();
    }
}

Ret UserConfigurationNavigation::enableCustom()
{
    m_enableCustom = true;

    return Ret::SUCCESS;
}

Ret UserConfigurationNavigation::disableCustom()
{
    Ret ret = Ret::SUCCESS;

    m_enableCustom = false;

    if(m_currentLayoutConfiguration.has_value() && !m_currentLayoutConfiguration.value().isHid())
    {
        auto associatedHid = m_currentLayoutConfiguration.value()
                                 .getAssociatedHidLayoutConfigurationId();

        // At this point we know that associatedHid has value
        ret = applyId(uuid::UuidView{associatedHid.value().get()});
    }

    return ret;
}

Ret UserConfigurationNavigation::applyFirst()
{
    Ret ret = Ret::NOT_FOUND;

    if(!m_userConfiguration.has_value())
    {
        return ret;
    }

    m_layoutConfigurationIt = m_userConfiguration.value().getLayoutConfigurationList().begin();
    ret = applyIterator(m_userConfiguration.value().getLayoutConfigurationList(),
                        m_layoutConfigurationIt);

    if(ret != Ret::SUCCESS)
    {
        ret = applyNext(SwitchDirection::FORWARD);
    }

    return ret;
}

Ret UserConfigurationNavigation::applyNext(SwitchDirection direction)
{
    if(!m_userConfiguration.has_value())
    {
        return Ret::NOT_FOUND;
    }

    Ret ret = Ret::UNEXPECTED;

    auto list = m_userConfiguration.value().getLayoutConfigurationList();

    auto initIt = m_layoutConfigurationIt;

    do
    {
        m_layoutConfigurationIt = iterate(list, m_layoutConfigurationIt, direction);
        ret = applyIterator(list, m_layoutConfigurationIt);
    } while(ret != Ret::SUCCESS && m_layoutConfigurationIt != initIt);

    return ret;
}

Ret UserConfigurationNavigation::applyId(uuid::UuidView id)
{
    if(!m_userConfiguration.has_value())
    {
        return Ret::NOT_FOUND;
    }

    Ret ret = Ret::SUCCESS;

    auto list = m_userConfiguration.value().getLayoutConfigurationList();
    auto it = findLayoutConfigurationId(id, list);

    if(it == list.end())
    {
        ret = Ret::NOT_FOUND;
    }

    if(ret == Ret::SUCCESS)
    {
        ret = applyIterator(list, it);
    }

    if(ret == Ret::SUCCESS)
    {
        m_layoutConfigurationIt = it;
    }

    return ret;
}

UserConfigurationNavigation::ConstIterator UserConfigurationNavigation::iterate(
    const etl::array_view<const LayoutConfigurationDescriptor>& list,
    const ConstIterator& it,
    SwitchDirection direction) const
{
    ConstIterator retIt = it;

    if(it == nullptr || it == list.end())
    {
        retIt = list.begin();
    }
    else if(!list.empty())
    {
        if(direction == SwitchDirection::FORWARD)
        {
            ++retIt;
            if(retIt == list.end())
            {
                retIt = list.begin();
            }
        }
        else if(direction == SwitchDirection::BACKWARD)
        {
            if(retIt == list.begin())
            {
                retIt = etl::prev(list.end());
            }
            else
            {
                retIt = etl::prev(retIt);
            }
        }
    }

    return retIt;
}

Ret UserConfigurationNavigation::applyIterator(
    const etl::array_view<const LayoutConfigurationDescriptor>& list, ConstIterator it)
{
    Ret ret = Ret::UNEXPECTED;

    if(it != list.end())
    {
        m_tempLayoutConfiguration.reset();
        ret = m_storageContext.getUserConfigurationStorage()
                  .getLayoutConfiguration(m_userConfiguration.value().getId(),
                                          uuid::UuidView{it->id},
                                          m_tempLayoutConfiguration);
    }

    if(ret == Ret::SUCCESS && m_tempLayoutConfiguration.has_value())
    {
        if(!m_enableCustom && !m_tempLayoutConfiguration.value().isHid())
        {
            ret = Ret::STATE;
        }
        else
        {
            ret = applyLayoutConfiguration(m_tempLayoutConfiguration.value());
        }

        if(ret == Ret::SUCCESS)
        {
            m_currentLayoutConfiguration.emplace(m_tempLayoutConfiguration.value());

            m_confChangeNotifier.notifyConfigurationChanged(&m_currentLayoutConfiguration.value());
        }
    }

    return ret;
}

Ret UserConfigurationNavigation::applyLayoutConfiguration(
    const LayoutConfiguration& layoutConfiguration)
{
    return m_application.applyLayoutConfiguration(m_storageContext.getLayoutStorage(),
                                                  layoutConfiguration)
               ? Ret::SUCCESS
               : Ret::UNEXPECTED;
}

UserConfigurationNavigation::ConstIterator UserConfigurationNavigation::findLayoutConfigurationId(
    uuid::UuidView id, const etl::array_view<const LayoutConfigurationDescriptor>& list) const
{
    auto it = etl::find_if(list.begin(),
                           list.end(),
                           [id](const configuration::LayoutConfigurationDescriptor& desc)
                           { return uuid::UuidView{desc.id} == id; });

    return it;
}