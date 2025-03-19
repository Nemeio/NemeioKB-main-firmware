#pragma once

#include "Uuid.hpp"
#include "etl/optional.h"
#include "etl/functional.h"

class IUuidList
{
public:
    IUuidList() = default;
    virtual ~IUuidList() = default;

    class Iterator
    {
    public:
        Iterator() = default;
        virtual ~Iterator() = default;

        virtual etl::optional<uuid::Uuid> next() = 0;
    };

    virtual etl::optional<etl::reference_wrapper<Iterator>> createIterator() = 0;
    virtual void deleteIterator(Iterator& iterator) = 0;
};