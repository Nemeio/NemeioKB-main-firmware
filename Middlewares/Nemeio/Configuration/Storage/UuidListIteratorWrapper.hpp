#pragma once

#include "IUuidList.hpp"
#include "cpp_helper.hpp"

class UuidListIteratorWrapper
{
public:
    explicit UuidListIteratorWrapper(IUuidList& list);
    COPYABLE_MOVABLE(UuidListIteratorWrapper, delete);
    ~UuidListIteratorWrapper();

    etl::optional<etl::reference_wrapper<IUuidList::Iterator>> it() { return m_iterator; }

private:
    IUuidList& m_list;
    etl::optional<etl::reference_wrapper<IUuidList::Iterator>> m_iterator;
};