/*
 * DFUPacketGenerator.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_COMMANDS_DFUPACKET_HPP_
#define NEMEIO_NRFDFU_COMMANDS_DFUPACKET_HPP_

#include <array_view.hpp>
#include <cstddef>
#include <array>
#include <cstdint>
#include <cstring>
#include "utils.hpp"
#include "IDFUPacket.hpp"

/* Execept for write packet, DFU packet should have a maximum size this */
static constexpr size_t DFU_PACKET_NORMALIZE_SIZE = 20;

template<size_t SIZE>
class DFUPacket : public IDFUPacket
{
public:
    DFUPacket();
    explicit DFUPacket(array_view<const uint8_t> content);
    virtual ~DFUPacket() = default;

    /* DFU Packet */
    const uint8_t* data();
    [[nodiscard]] size_t size();

    /* DFU Payload */
    DFUCommand getCommand();
    const uint8_t* payloadData();
    size_t payloadSize();

protected:
    bool createWithPayload(DFUCommand command, array_view<const uint8_t> array);

private:
    struct __attribute__((__packed__)) s_dfuPacket
    {
        DFUCommand command;
        uint8_t payload[];
    };

    std::array<uint8_t, SIZE + sizeof(s_dfuPacket)> mPacket;
    size_t mPacketSize = 0;

    void reset();
};

template<size_t SIZE>
void DFUPacket<SIZE>::reset()
{
    mPacket.fill(0);
    mPacketSize = 0;
}

template<size_t SIZE>
DFUPacket<SIZE>::DFUPacket()
{
    reset();
}

template<size_t SIZE>
DFUPacket<SIZE>::DFUPacket(array_view<const uint8_t> array)
{
    if(mPacket.size() >= array.size())
    {
        mPacketSize = array.size();
        memcpy(mPacket.data(), array.data(), mPacketSize);
    }
    else
    {
        mPacketSize = 0;
    }
}

template<size_t SIZE>
bool DFUPacket<SIZE>::createWithPayload(DFUCommand command, array_view<const uint8_t> array)
{
    reset();
    if(array.size() + member_size(s_dfuPacket, command) > mPacket.size())
    {
        return false;
    }

    auto packet = reinterpret_cast<s_dfuPacket*>(mPacket.data());
    packet->command = command;
    if(array.size() > 0)
    {
        memcpy(packet->payload, array.data(), array.size());
    }
    mPacketSize = array.size() + member_size(s_dfuPacket, command);
    return true;
}

template<size_t SIZE>
const uint8_t* DFUPacket<SIZE>::data()
{
    return mPacket.data();
}

template<size_t SIZE>
size_t DFUPacket<SIZE>::size()
{
    return mPacketSize;
}

template<size_t SIZE>
DFUCommand DFUPacket<SIZE>::getCommand()
{
    DFUCommand command = DFUCommand::UNKNOWN;

    if(mPacketSize >= member_size(s_dfuPacket, command))
    {
        command = (reinterpret_cast<s_dfuPacket*>(mPacket.data()))->command;
    }

    return command;
}

template<size_t SIZE>
const uint8_t* DFUPacket<SIZE>::payloadData()
{
    if(mPacketSize < member_size(s_dfuPacket, command))
    {
        return nullptr;
    }

    return (reinterpret_cast<s_dfuPacket*>(mPacket.data()))->payload;
}

template<size_t SIZE>
size_t DFUPacket<SIZE>::payloadSize()
{
    if(mPacketSize < member_size(s_dfuPacket, command))
    {
        return 0;
    }

    return mPacketSize - member_size(s_dfuPacket, command);
}

#endif /* NEMEIO_NRFDFU_COMMANDS_DFUPACKET_HPP_ */
