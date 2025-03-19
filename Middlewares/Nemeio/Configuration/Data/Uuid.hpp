#pragma once

#include "etl/array.h"
#include "etl/array_view.h"
#include "cpp_helper.hpp"
#include "embedded_ostream.hpp"

namespace uuid
{
inline constexpr size_t SIZE = 16;

class UuidView;

class Uuid
{
public:
    Uuid() = delete;
    explicit Uuid(etl::array_view<const std::byte> value);
    explicit Uuid(UuidView value);
    etl::array_view<const std::byte> get() const;

    bool operator==(const Uuid& uuid) const;
    bool operator==(const UuidView& uuid) const;
    bool operator!=(const Uuid& uuid) const;
    bool operator!=(const UuidView& uuid) const;

private:
    etl::array<std::byte, SIZE> m_value;

    friend embedded_ostream& operator<<(embedded_ostream& os, const Uuid& uuid);
};

class UuidView
{
public:
    UuidView() = delete;
    explicit UuidView(const Uuid& uuid);
    explicit UuidView(etl::array_view<const std::byte> view);

    COPYABLE_MOVABLE(UuidView, default);

    etl::array_view<const std::byte> get() const;

    bool operator==(const Uuid& uuid) const;
    bool operator==(const UuidView& uuid) const;
    bool operator!=(const Uuid& uuid) const;
    bool operator!=(const UuidView& uuid) const;

private:
    const etl::array_view<const std::byte> m_view;

    friend embedded_ostream& operator<<(embedded_ostream& os, const UuidView& uuid);
};
} // namespace uuid