#include "KeyboardStatus.hpp"


KeyboardStatus::KeyboardStatus(logger::ITraceLogger& logger)
    : m_logger(logger)
{
}

void KeyboardStatus::refresh()
{
    AutoLock lock(m_logger);
    m_logSize = m_logger.oss().size();
    m_isErrorLogged = m_logger.isErrorLogged();
};