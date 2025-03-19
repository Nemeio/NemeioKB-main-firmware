#pragma once

#include <etl/hfsm.h>
#include "Mutex.hpp"

class Hfsm : public etl::hfsm
{
public:
    using etl::hfsm::hfsm;

    void receive(const etl::imessage& msg_) final
    {
        AutoLock autoLock(m_mutex);
        etl::hfsm::receive(msg_);
    }

private:
    Mutex m_mutex;
};