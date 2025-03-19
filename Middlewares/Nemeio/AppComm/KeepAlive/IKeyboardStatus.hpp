#pragma once

#include <cstddef>

class IKeyboardStatus
{
public:
    IKeyboardStatus() = default;
    virtual ~IKeyboardStatus() = default;

    virtual void refresh() = 0;
    virtual size_t getLogSize() const = 0;
    virtual bool isErrorLogged() const = 0;
};