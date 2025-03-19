#pragma once

#include "keyIDConfig.h"

class IDoubleClickKeyListener
{
public:
    IDoubleClickKeyListener() = default;
    virtual ~IDoubleClickKeyListener() = default;

    virtual void simpleClicked(const ButtonID& button) = 0;
    virtual void doubleClicked(const ButtonID& button) = 0;
};
