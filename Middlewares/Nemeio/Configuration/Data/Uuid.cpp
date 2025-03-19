#include "Uuid.hpp"
#include "utils.hpp"

using namespace uuid;

Uuid::Uuid(etl::array_view<const std::byte> value)
{
    if(value.size() == m_value.size())
    {
        m_value.assign(value.begin(), value.end());
    }
}

Uuid::Uuid(UuidView value)
    : Uuid(value.get())
{
}

etl::array_view<const std::byte> Uuid::get() const
{
    return m_value;
}

bool Uuid::operator==(const Uuid& uuid) const
{
    return uuid.m_value == m_value;
}

bool Uuid::operator==(const UuidView& uuid) const
{
    return uuid.get() == m_value;
}

bool Uuid::operator!=(const Uuid& uuid) const
{
    return !(uuid == *this);
}
bool Uuid::operator!=(const UuidView& uuid) const
{
    return !(uuid == *this);
}

UuidView::UuidView(const Uuid& uuid)
    : m_view(uuid.get())
{
}

UuidView::UuidView(etl::array_view<const std::byte> view)
    : m_view(view)
{
}

etl::array_view<const std::byte> UuidView::get() const
{
    return m_view;
}

bool UuidView::operator==(const Uuid& uuid) const
{
    return uuid == *this;
}

bool UuidView::operator==(const UuidView& uuid) const
{
    return uuid.m_view == m_view;
}

bool UuidView::operator!=(const Uuid& uuid) const
{
    return !(uuid == *this);
}
bool UuidView::operator!=(const UuidView& uuid) const
{
    return !(uuid == *this);
}

namespace uuid
{
embedded_ostream& operator<<(embedded_ostream& os, const Uuid& uuid)
{
    uuid::UuidView view(uuid);
    os << view;
    return os;
}

embedded_ostream& operator<<(embedded_ostream& os, const UuidView& uuid)
{
    for(auto byte: uuid.m_view)
    {
        auto str = byteToHexStr(byte);
        os << str.data();
    }

    return os;
}
} // namespace uuid