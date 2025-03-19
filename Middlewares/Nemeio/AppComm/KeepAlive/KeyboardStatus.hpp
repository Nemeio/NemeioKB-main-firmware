#pragma once

#include "IKeyboardStatus.hpp"
#include "ITraceLogger.hpp"
#include "AutoLock.h"

class KeyboardStatus : public IKeyboardStatus
{
public:
    explicit KeyboardStatus(logger::ITraceLogger& logger);
    ~KeyboardStatus() final = default;

    void refresh() final;
    size_t getLogSize() const final { return m_logSize; }
    bool isErrorLogged() const final { return m_isErrorLogged; }

private:
    logger::ITraceLogger& m_logger;
    size_t m_logSize;
    bool m_isErrorLogged{false};
};