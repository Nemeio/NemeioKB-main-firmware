#pragma once

#include "etl/optional.h"
#include "etl/functional.h"

template<typename T>
using OptionalReturn = etl::optional<etl::reference_wrapper<etl::optional<T>>>;

template<typename T>
inline OptionalReturn<T> createOptionalReturn(etl::optional<T>& object)
{
    etl::reference_wrapper<etl::optional<T>> refWrapper(object);
    return OptionalReturn<T>(refWrapper);
}