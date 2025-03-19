/*
 * array_view.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_TOOLS_ARRAY_VIEW_HPP_
#define NEMEIO_TOOLS_ARRAY_VIEW_HPP_

#include <vector>
#include "assertTools.h"

template<typename T>
class array_view
{
public:
    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;

        explicit const_iterator(pointer ptr)
            : ptr_(ptr)
        {
        }
        self_type operator++()
        {
            self_type i = *this;
            ptr_++;
            return i;
        }
        self_type operator++(int junk)
        {
            ptr_++;
            return *this;
        }
        const reference operator*() { return *ptr_; }
        pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }

    private:
        pointer ptr_;
    };


    const_iterator begin() const { return const_iterator(mDataPtr); }

    const_iterator end() const { return const_iterator(mDataPtr + mNbElem); }

    array_view(T* data, std::size_t size);

    T& operator[](int index);

    std::size_t size() const;
    T* data() const;

private:
    T* mDataPtr;
    std::size_t mNbElem;
};

template<typename T>
array_view<T>::array_view(T* data, std::size_t size)
    : mDataPtr(data)
    , mNbElem(size)
{
}

template<typename T>
std::size_t array_view<T>::size() const
{
    return mNbElem;
}

template<typename T>
T* array_view<T>::data() const
{
    return mDataPtr;
}

template<typename T>
T& array_view<T>::operator[](int index)
{
    ASSERT(index >= 0 && index < static_cast<int>(mNbElem));
    return mDataPtr[index];
}

#endif /* NEMEIO_TOOLS_ARRAY_VIEW_HPP_ */
