/*
 * utils.h
 *
 *  Created on: Jan 24, 2019
 *      Author: Labo
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <iterator>
#include <string.h>
#include "assertTools.h"
#include <variant>
#include <climits>
#include <cmath>
#include "embedded_ostream.hpp"
#include "etl/string.h"
#include "etl/variant.h"
#include "etl/overload.h"
#include "etl/optional.h"
#include "etl/functional.h"

#ifdef _MSC_VER
#define NOMINMAX
#include <Windows.h>
#endif

#ifndef SET_BIT
#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#endif

#define member_size(type, member) sizeof(((type*) 0)->member)

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static inline constexpr uint8_t BITS_PER_BYTE = 8;
static inline constexpr int32_t NB_MILLISECS_IN_SECS = 1000;
static inline constexpr int32_t NB_MICROSECS_IN_MILLISECS = 1000;

template<typename T, std::size_t N>
constexpr std::size_t arrayCount(T const (&)[N]) noexcept
{
    return N;
}

char* strncpyNullTerminated(char* destination, const char* source, size_t num);

bool charToDigit(char c, uint8_t& i);

etl::string<2> byteToHexStr(std::byte byte);

template<typename T1, typename T2>
inline T1 divRoundUp(T1 x, T2 y)
{
    return (x + y - 1) / y;
}

void bitRightShiftInByteArray(const uint8_t* buffSrc,
                              const std::size_t lenSrc,
                              const uint8_t rightShift,
                              uint8_t* buffDst,
                              const std::size_t lenDst,
                              bool defaultBitsIsOne);

#ifdef __GNUC__
#if(!defined(__BYTE_ORDER__))
#error "Byte order unknown"
#elif __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error "LE systems only"
#endif
#elif defined(_MSC_VER)
#if(!defined(REG_DWORD))
#error "Byte order unknown"
#elif(REG_DWORD != REG_DWORD_LITTLE_ENDIAN)
#error "LE systems only"
#endif
#endif

inline uint32_t BE32ToHost(uint32_t be32)
{
    uint8_t* pBe32 = reinterpret_cast<uint8_t*>(&be32);
    return (pBe32[0] << 24) | (pBe32[1] << 16) | (pBe32[2] << 8) | (pBe32[3] << 0);
}

inline uint32_t LE32ToHost(uint32_t le32)
{
    // LE systems only, no conversion needed
    return le32;
}

inline uint16_t BE16ToHost(uint16_t be16)
{
    uint8_t* pBe16 = reinterpret_cast<uint8_t*>(&be16);
    return (pBe16[0] << 8) | (pBe16[1] << 0);
}

inline uint32_t HostToBE32(uint32_t u32)
{
    uint32_t be32 = 0;
    uint8_t* pBE32 = reinterpret_cast<uint8_t*>(&be32);
    pBE32[0] = (u32 >> 24) & 0xFF;
    pBE32[1] = (u32 >> 16) & 0xFF;
    pBE32[2] = (u32 >> 8) & 0xFF;
    pBE32[3] = u32 & 0xFF;
    return be32;
}

inline uint16_t HostToBE16(uint16_t u16)
{
    uint16_t be16 = 0;
    uint8_t* pBE16 = reinterpret_cast<uint8_t*>(&be16);
    pBE16[0] = (u16 >> 8) & 0xFF;
    pBE16[1] = u16 & 0xFF;
    return be16;
}

template<typename BaseType, typename... Types>
BaseType& castToBase(std::variant<Types...>& v)
{
    return std::visit([](BaseType& arg) -> BaseType& { return arg; }, v);
}

template<typename BaseType, typename... Types>
BaseType& castToBase(const std::variant<Types...>& v)
{
    return std::visit([](BaseType& arg) -> BaseType& { return arg; }, v);
}

template<typename BaseType, typename... Types>
BaseType& castToBase(etl::variant<Types...>& v)
{
    etl::optional<etl::reference_wrapper<BaseType>> ret;
    auto visitor = etl::make_overload([&ret](BaseType& arg) { ret.emplace(arg); });
    v.accept_functor(visitor);
    return ret.value().get();
}

template<typename BaseType, typename... Types>
const BaseType& castToBase(const etl::variant<Types...>& v)
{
    // Const accept_functor method is not currently provided by ETL. However, this method does not modifiy the variant.
    // As a workaround, cast the variant as non-const and force constness of visitor overload argument.
    etl::optional<etl::reference_wrapper<const BaseType>> ret;
    auto visitor = etl::make_overload([&ret](const BaseType& arg) { ret.emplace(arg); });
    const_cast<etl::variant<Types...>&>(v).accept_functor(visitor);
    return ret.value().get();
}

template<class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// UNPACKARGS macro is needed to allow commas in structure declaration
#define UNPACKARGS(...) __VA_ARGS__

#ifdef __GNUC__
#define PACK(__Declaration__) UNPACKARGS __Declaration__ __attribute__((__packed__))
#elif defined(_MSC_VER)
#define PACK(__Declaration__) __pragma(pack(push, 1)) UNPACKARGS __Declaration__ __pragma(pack(pop))
#endif

#ifdef __GNUC__
#define PACK_ALIGN(__Declaration__, __Alignement__) \
    UNPACKARGS __Declaration__ __attribute__((__packed__)) __attribute__((aligned(__Alignement__)))
#elif defined(_MSC_VER)
#error Not implemented
#endif

inline uint16_t numberToPowerOfTwo(uint32_t value)
{
    int powerOfTwo = __builtin_ctz(value);
    ASSERT((static_cast<uint32_t>(1 << powerOfTwo)) == value);

    return powerOfTwo;
}

template<typename T>
constexpr T KO_TO_BYTES(T value)
{
    return (value * 1024);
}

template<typename T>
constexpr T MO_TO_BYTES(T value)
{
    return (KO_TO_BYTES(value) * 1024);
}

#endif /* UTILS_HPP_ */
