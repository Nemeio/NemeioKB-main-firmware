#pragma once

#include "etl/observer.h"

struct UserConfigurationDeleteEvent
{
    uuid::UuidView id;
};

using UserConfigurationStorageObserver = etl::observer<const UserConfigurationDeleteEvent&>;