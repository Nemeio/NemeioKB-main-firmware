/*
 * ConcatenatedCollection.hpp
 *
 *  Created on: Nov 14, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_TOOLS_CONCATENATEDCOLLECTION_CONCATENATEDCOLLECTION_HPP_
#define NEMEIO_TOOLS_CONCATENATEDCOLLECTION_CONCATENATEDCOLLECTION_HPP_

#include <functional>

template<typename Collection, typename T>
class ConcatenatedCollection
{
public:
    typedef size_t size_type;

    class iterator
    {
    public:
        typedef iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef typename Collection::iterator subiterator;
        typedef std::forward_iterator_tag iterator_category;
        iterator(subiterator ptr, Collection& coll, Collection& coll1, Collection& coll2)
            : mCollIterator(ptr)
            , mCurrentColl(std::ref(coll))
            , mColl1(std::ref(coll1))
            , mColl2(std::ref(coll2))
        {
        }
        self_type operator++()
        {
            ++mCollIterator;
            if(&mCurrentColl.get() == &mColl1.get() && mCollIterator == mColl1.get().end())
            {
                mCollIterator = mColl2.get().begin();
                mCurrentColl = std::ref(mColl2);
            }
            return *this;
        }
        self_type operator++(int junk)
        {
            self_type i = *this;
            ++*this;
            return i;
        }
        reference operator*() { return *mCollIterator; }
        pointer operator->() { return mCollIterator; }
        bool operator==(const self_type& rhs)
        {
            return &mCurrentColl.get() == &rhs.mCurrentColl.get()
                   && &mColl1.get() == &rhs.mColl1.get() && &mColl2.get() == &rhs.mColl2.get()
                   && mCollIterator == rhs.mCollIterator;
        }
        bool operator!=(const self_type& rhs) { return !(*this == rhs); }

    private:
        subiterator mCollIterator;
        std::reference_wrapper<Collection> mCurrentColl;
        std::reference_wrapper<Collection> mColl1;
        std::reference_wrapper<Collection> mColl2;
    };

    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef typename Collection::iterator subiterator;
        typedef std::forward_iterator_tag iterator_category;
        const_iterator(subiterator ptr, Collection& coll, Collection& coll1, Collection& coll2)
            : mCollIterator(ptr)
            , mCurrentColl(std::ref(coll))
            , mColl1(std::ref(coll1))
            , mColl2(std::ref(coll2))
        {
        }
        self_type operator++()
        {
            ++mCollIterator;
            if(&mCurrentColl.get() == &mColl1.get() && mCollIterator == mColl1.get().end())
            {
                mCollIterator = mColl2.get().begin();
                mCurrentColl = std::ref(mColl2);
            }
            return *this;
        }
        self_type operator++(int junk)
        {
            self_type i = *this;
            ++*this;
            return i;
        }
        const reference operator*() { return *mCollIterator; }
        const pointer operator->() { return mCollIterator; }
        bool operator==(const self_type& rhs)
        {
            return &mCurrentColl.get() == &rhs.mCurrentColl.get()
                   && &mColl1.get() == &rhs.mColl1.get() && &mColl2.get() == &rhs.mColl2.get()
                   && mCollIterator == rhs.mCollIterator;
        }
        bool operator!=(const self_type& rhs) { return !(*this == rhs); }

    private:
        subiterator mCollIterator;
        std::reference_wrapper<Collection> mCurrentColl;
        std::reference_wrapper<Collection> mColl1;
        std::reference_wrapper<Collection> mColl2;
    };


    ConcatenatedCollection(Collection& coll1, Collection& coll2)
        : mColl1(coll1)
        , mColl2(coll2)
    {
    }

    size_type size() const { return mColl1.size() + mColl2.size(); }

    T& operator[](size_type index)
    {
        if(index < mColl1.size())
        {
            return mColl1[index];
        }
        else
        {
            index -= mColl1.size();
            return mColl2[index];
        }
    }

    const T& operator[](size_type index) const
    {
        if(index < mColl1.size())
        {
            return mColl1[index];
        }
        else
        {
            index -= mColl1.size();
            return mColl2[index];
        }
    }

    iterator begin()
    {
        // begin cannot refer to mColl1.end()
        auto begin = mColl1.size() > 0 ? mColl1.begin() : mColl2.begin();
        Collection& coll = mColl1.size() > 0 ? mColl1 : mColl2;
        return iterator(begin, coll, mColl1, mColl2);
    }

    iterator end() { return iterator(mColl2.end(), mColl2, mColl1, mColl2); }

    const_iterator begin() const
    {
        // begin cannot refer to mColl1.end()
        auto begin = mColl1.size() > 0 ? mColl1.begin() : mColl2.begin();
        Collection& coll = mColl1.size() > 0 ? mColl1 : mColl2;
        return const_iterator(begin, coll, mColl1, mColl2);
    }

    const_iterator end() const { return const_iterator(mColl2.end(), mColl2, mColl1, mColl2); }

    void clear()
    {
        mColl1.clear();
        mColl2.clear();
    }

private:
    Collection& mColl1;
    Collection& mColl2;
};

#endif /* NEMEIO_TOOLS_CONCATENATEDCOLLECTION_CONCATENATEDCOLLECTION_HPP_ */
