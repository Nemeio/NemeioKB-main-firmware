/*
 * WriteBuffer.hpp
 *
 *  Created on: May 27, 2019
 *      Author: Labo
 */

#ifndef TOOLS_WRITEBUFFER_HPP_
#define TOOLS_WRITEBUFFER_HPP_

#include <array>
#include <cstddef>

template<typename T, std::size_t N>
class WriteBuffer
{
public:
    WriteBuffer();
    virtual ~WriteBuffer() = default;

    bool isEmpty() const;
    bool isFull() const;
    std::size_t usedSpace() const;
    std::size_t remainingSpace() const;

    template<class InputIt>
    std::size_t pushBack(InputIt first, InputIt last);

    template<class DataType>
    std::size_t pushBack(const DataType& data);

    template<class DataType>
    std::size_t pushBack(const DataType& data, size_t size);

    template<typename DataType>
    std::size_t cleanWrite(const DataType& data);

    template<class InputIt>
    std::size_t cleanWrite(InputIt first, InputIt last);

    T* data();
    const T* data() const;
    std::array<T, N> array();
    constexpr std::size_t size() const;
    constexpr typename std::array<T, N>::const_iterator begin() const;
    typename std::array<T, N>::const_iterator end() const;

    void reset(const T& value);
    void reset();

private:
    std::array<T, N> mBuffer;
    std::size_t mBufferIdx;
};

template<typename T, std::size_t N>
inline bool WriteBuffer<T, N>::isEmpty() const
{
    return 0 == mBufferIdx;
}

template<typename T, std::size_t N>
inline bool WriteBuffer<T, N>::isFull() const
{
    return N == mBufferIdx;
}

template<typename T, std::size_t N>
inline std::size_t WriteBuffer<T, N>::usedSpace() const
{
    return mBufferIdx;
}

template<typename T, std::size_t N>
inline std::size_t WriteBuffer<T, N>::remainingSpace() const
{
    return N - mBufferIdx;
}

template<typename T, std::size_t N>
inline T* WriteBuffer<T, N>::data()
{
    return mBuffer.data();
}

template<typename T, std::size_t N>
inline const T* WriteBuffer<T, N>::data() const
{
    return mBuffer.data();
}

template<typename T, std::size_t N>
inline std::array<T, N> WriteBuffer<T, N>::array()
{
    return mBuffer;
}

template<typename T, std::size_t N>
inline constexpr std::size_t WriteBuffer<T, N>::size() const
{
    return N;
}

template<typename T, std::size_t N>
inline WriteBuffer<T, N>::WriteBuffer()
{
    mBufferIdx = 0;
}

template<typename T, std::size_t N>
inline void WriteBuffer<T, N>::reset(const T& value)
{
    mBuffer.fill(value);
    reset();
}

template<typename T, std::size_t N>
inline constexpr typename std::array<T, N>::const_iterator WriteBuffer<T, N>::begin() const
{
    return mBuffer.begin();
}

template<typename T, std::size_t N>
inline typename std::array<T, N>::const_iterator WriteBuffer<T, N>::end() const
{
    return mBuffer.begin() + mBufferIdx;
}

template<typename T, std::size_t N>
inline void WriteBuffer<T, N>::reset()
{
    mBufferIdx = 0;
}

template<typename T, std::size_t N>
template<class InputIt>
inline std::size_t WriteBuffer<T, N>::pushBack(InputIt first, InputIt last)
{
    std::size_t written = 0;

    std::size_t writeLen = std::distance(first, last);
    if(mBufferIdx + writeLen <= N)
    {
        std::copy(first, last, mBuffer.begin() + mBufferIdx);
        mBufferIdx += writeLen;
        written = writeLen;
    }

    return written;
}

template<typename T, std::size_t N>
template<class DataType>
inline std::size_t WriteBuffer<T, N>::pushBack(const DataType& data)
{
    static_assert(sizeof(DataType) % sizeof(T) == 0, "DataType must be a multiple of T");

    constexpr std::size_t dataSize = sizeof(DataType) / sizeof(T);
    static_assert(N >= dataSize, "Buffer too small");

    const T* pData = reinterpret_cast<const T*>(&data);

    return pushBack(pData, pData + dataSize);
}

template<typename T, std::size_t N>
template<class DataType>
inline std::size_t WriteBuffer<T, N>::pushBack(const DataType& data, size_t size)
{
    if(size > sizeof(DataType))
    {
        return 0;
    }

    const T* pData = reinterpret_cast<const T*>(&data);

    return pushBack(pData, pData + size);
}

template<typename T, std::size_t N>
template<typename DataType>
inline std::size_t WriteBuffer<T, N>::cleanWrite(const DataType& data)
{
    reset();
    return pushBack(data);
}

template<typename T, std::size_t N>
template<class InputIt>
inline std::size_t WriteBuffer<T, N>::cleanWrite(InputIt first, InputIt last)
{
    reset();
    return pushBack(first, last);
}

#endif /* TOOLS_WRITEBUFFER_HPP_ */
