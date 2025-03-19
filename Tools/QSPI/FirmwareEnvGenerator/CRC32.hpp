#ifndef CRC32_STM32CRC32_HPP_
#define CRC32_STM32CRC32_HPP_

#include "ICRC32.hpp"
#include "RecursiveMutex.hpp"

class CRC32 : public ICRC32
{
public:
    CRC32() = default;
    ~CRC32() final = default;

    void configure() final;
    bool compute(const uint8_t* buffer, size_t len, uint32_t& crc32) final;

    bool lock(DelayMs timeout) final;
    bool unlock() final;

private:
    static uint32_t mTable[];
    RecursiveMutex mMutex;
};

#endif /* CRC32_STM32CRC32_HPP_ */
