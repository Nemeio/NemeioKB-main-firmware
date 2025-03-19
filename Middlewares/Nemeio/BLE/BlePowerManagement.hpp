#pragma once

#include "BleEventObserver.hpp"
#include "IPowerManager.hpp"

class BlePowerManagement : public BleEventObserver
{
public:
    explicit BlePowerManagement(IPowerManager& powerManager);
    ~BlePowerManagement() final = default;

    void notification(BleEvent event) final;

private:
    IPowerManager& m_powerManager;
};