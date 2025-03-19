#pragma once

#include "etl/observer.h"

struct UserConfigurationUpdateEvent
{
    uuid::UuidView id;
};

using UserConfigurationUpdateObserver = etl::observer<const UserConfigurationUpdateEvent&>;