#pragma once

#include "etl/observer.h"
#include "Mutex.hpp"
#include "AutoLock.h"
#include "cpp_helper.hpp"

template<typename TObserver, const size_t MAX_OBSERVERS>
class Observable : public etl::observable<TObserver, MAX_OBSERVERS>
{
public:
    Observable() = default;
    virtual ~Observable() = default;

    bool try_add_observer(TObserver& observer)
    {
        bool ret = (etl::observable<TObserver, MAX_OBSERVERS>::number_of_observers()
                    < MAX_OBSERVERS);

        if(ret)
        {
            etl::observable<TObserver, MAX_OBSERVERS>::add_observer(observer);
        }

        return ret;
    }
};

template<typename TObserver, const size_t MAX_OBSERVERS>
class ProtectedObservable : public Observable<TObserver, MAX_OBSERVERS>
{
public:
    ProtectedObservable() = default;
    virtual ~ProtectedObservable() { AutoLock autoLock(m_mutex); }
    COPYABLE_MOVABLE(ProtectedObservable, delete);

    void add_observer(TObserver& observer)
    {
        AutoLock autoLock(m_mutex);

        Observable<TObserver, MAX_OBSERVERS>::add_observer(observer);
    }

    bool try_add_observer(TObserver& observer)
    {
        return Observable<TObserver, MAX_OBSERVERS>::try_add_observer(observer);
    }

    bool remove_observer(TObserver& observer)
    {
        AutoLock autoLock(m_mutex);

        return Observable<TObserver, MAX_OBSERVERS>::remove_observer(observer);
    }

    void enable_observer(TObserver& observer, bool state = true)
    {
        AutoLock autoLock(m_mutex);

        Observable<TObserver, MAX_OBSERVERS>::enable_observer(observer, state);
    }

    void disable_observer(TObserver& observer)
    {
        AutoLock autoLock(m_mutex);

        Observable<TObserver, MAX_OBSERVERS>::disable_observer(observer);
    }

    void clear_observers()
    {
        AutoLock autoLock(m_mutex);

        Observable<TObserver, MAX_OBSERVERS>::clear_observers();
    }

    typename etl::observable<TObserver, MAX_OBSERVERS>::size_type number_of_observers() const
    {
        AutoLock autoLock(m_mutex);

        return Observable<TObserver, MAX_OBSERVERS>::number_of_observers();
    }

    template<typename TNotification>
    void notify_observers(TNotification&& n)
    {
        AutoLock autoLock(m_mutex);

        Observable<TObserver, MAX_OBSERVERS>::notify_observers(etl::forward<TNotification>(n));
    }

private:
    Mutex m_mutex;
};