#include "LayoutConfiguration.hpp"

using namespace configuration;

LayoutConfiguration::LayoutConfiguration(
    uuid::UuidView id,
    uuid::UuidView layoutHash,
    etl::optional<uuid::UuidView> associatedHidLayoutConfigurationId,
    OperatingSystem operatingSystem,
    etl::array_view<const KeySequence> winSasSequences,
    etl::array_view<const KeySequence> capslockSequences)
    : m_id(id)
    , m_layoutHash(layoutHash)
    , m_associatedHidLayoutConfigurationId(
          associatedHidLayoutConfigurationId.has_value()
              ? etl::optional<uuid::Uuid>(uuid::Uuid(associatedHidLayoutConfigurationId.value()))
              : etl::nullopt)
    , m_operatingSystem(operatingSystem)
{
    if(winSasSequences.size() <= m_winSasSequences.capacity())
    {
        m_winSasSequences.assign(winSasSequences.begin(), winSasSequences.end());
    }
    if(capslockSequences.size() <= m_capslockKeySequences.capacity())
    {
        m_capslockKeySequences.assign(capslockSequences.begin(), capslockSequences.end());
    }
}

bool LayoutConfiguration::operator==(const LayoutConfiguration& layoutConfiguration) const
{
    return m_id == layoutConfiguration.m_id && m_layoutHash == layoutConfiguration.m_layoutHash
           && m_associatedHidLayoutConfigurationId
                  == layoutConfiguration.m_associatedHidLayoutConfigurationId
           && m_operatingSystem == layoutConfiguration.m_operatingSystem
           && m_winSasSequences == layoutConfiguration.m_winSasSequences
           && m_capslockKeySequences == layoutConfiguration.m_capslockKeySequences;
}