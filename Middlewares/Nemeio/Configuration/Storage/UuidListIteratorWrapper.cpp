
#include "UuidListIteratorWrapper.hpp"

UuidListIteratorWrapper::UuidListIteratorWrapper(IUuidList& list)
    : m_list(list)
    , m_iterator(list.createIterator())
{
}

UuidListIteratorWrapper::~UuidListIteratorWrapper()
{
    if(m_iterator.has_value())
    {
        m_list.deleteIterator(m_iterator.value().get());
    }
}