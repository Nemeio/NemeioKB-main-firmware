#pragma once

#include "etl/string.h"
#include "etl/array_view.h"
#include "etl/vector.h"
#include "etl/optional.h"
#include "etl/functional.h"
#include "KeySequence.hpp"
#include "Uuid.hpp"

namespace configuration
{
enum class OperatingSystem
{
    UNDEFINED = 0,
    WINDOWS,
    MAC,
    MAX
};

class LayoutConfiguration
{
public:
    static constexpr size_t MAX_WIN_SAS_SEQUENCES = 15;
    static constexpr size_t MAX_CAPSLOCK_SEQUENCES = 1;
    LayoutConfiguration(uuid::UuidView id,
                        uuid::UuidView layoutHash,
                        etl::optional<uuid::UuidView> associatedHidLayoutConfigurationId,
                        OperatingSystem operatingSystem,
                        etl::array_view<const KeySequence> winSasSequences,
                        etl::array_view<const KeySequence> capslockSequences);
    virtual ~LayoutConfiguration() = default;

    uuid::UuidView getId() const { return uuid::UuidView(m_id); }
    uuid::UuidView getLayoutHash() const { return uuid::UuidView(m_layoutHash); }
    etl::optional<etl::reference_wrapper<const uuid::Uuid>> getAssociatedHidLayoutConfigurationId()
        const
    {
        return m_associatedHidLayoutConfigurationId.has_value()
                   ? etl::optional<etl::reference_wrapper<const uuid::Uuid>>(
                       etl::cref(m_associatedHidLayoutConfigurationId.value()))
                   : etl::nullopt;
    }
    OperatingSystem getOperatingSystem() const { return m_operatingSystem; }
    etl::array_view<const KeySequence> getWinSasSequence() const { return m_winSasSequences; }
    etl::array_view<const KeySequence> getCapslockKeySequences() const
    {
        return m_capslockKeySequences;
    }

    bool isHid() const { return !m_associatedHidLayoutConfigurationId.has_value(); }

    bool operator==(const LayoutConfiguration& layoutConfiguration) const;

private:
    uuid::Uuid m_id;
    uuid::Uuid m_layoutHash;
    etl::optional<uuid::Uuid> m_associatedHidLayoutConfigurationId{etl::nullopt};
    OperatingSystem m_operatingSystem{OperatingSystem::UNDEFINED};
    etl::vector<KeySequence, MAX_WIN_SAS_SEQUENCES> m_winSasSequences{};
    etl::vector<KeySequence, MAX_CAPSLOCK_SEQUENCES> m_capslockKeySequences{};
};
} // namespace configuration