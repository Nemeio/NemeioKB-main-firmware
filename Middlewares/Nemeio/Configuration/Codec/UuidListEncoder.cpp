#include "UuidListEncoder.hpp"
#include "UuidListIteratorWrapper.hpp"

UuidListEncoder::UuidListEncoder(IUuidListCodec& uuidListCodec)
    : m_uuidListCodec(uuidListCodec)
{
}

bool UuidListEncoder::encode(IUuidList& list)
{
    UuidListIteratorWrapper itWrapper(list);

    if(!itWrapper.it().has_value())
    {
        return false;
    }

    auto& it = itWrapper.it().value().get();
    m_vectorList.clear();
    etl::fill(buffer().begin(), buffer().end(), std::byte(0));

    auto id = it.next();
    while(id.has_value())
    {
        m_vectorList.push_back(id.value());
        id = it.next();
    }

    m_encodedSize = m_uuidListCodec.encode(m_vectorList, buffer());

    return m_encodedSize >= 0;
}