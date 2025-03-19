#pragma once

#include "UuidListEncoder.hpp"
#include "Mutex.hpp"
#include "AutoLock.h"
#include "etl/pool.h"

class UuidListEncoderContainer
{
public:
    explicit UuidListEncoderContainer(IUuidListCodec& uuidListCodec)
        : m_uuidListCodec(uuidListCodec)
    {
    }
    virtual ~UuidListEncoderContainer() = default;

    etl::optional<etl::reference_wrapper<UuidListEncoder>> createEncoder()
    {
        etl::optional<etl::reference_wrapper<UuidListEncoder>> ret;
        AutoLock autoLock(m_mutex);

        if(UuidListEncoder* it = m_pool.create(m_uuidListCodec); it != nullptr)
        {
            ret.emplace(*it);
        }

        return ret;
    }

    void deleteEncoder(const UuidListEncoder& encoder)
    {
        AutoLock autoLock(m_mutex);
        m_pool.destroy(&encoder);
    }

private:
    static constexpr size_t NB_ENCODERS = 1;

    Mutex m_mutex;
    etl::pool<UuidListEncoder, NB_ENCODERS> m_pool;
    IUuidListCodec& m_uuidListCodec;
};