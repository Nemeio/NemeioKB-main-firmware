#include "BlePowerManagement.hpp"
#include "IPowerManager.hpp"

BlePowerManagement::BlePowerManagement(IPowerManager& powerManager)
    : m_powerManager(powerManager)
{
}
void BlePowerManagement::notification(BleEvent event)
{
    switch(event)
    {
    case BleEvent::CONNECTED:
    case BleEvent::DISCONNECTED:
        m_powerManager.systemActivityHappened();
        break;
    default:
        break;
    }
}