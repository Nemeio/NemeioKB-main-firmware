#pragma once

#include "IConnectivityListener.hpp"
#include "IBleChip.hpp"

class BleActivationStrategy : public IConnectivityListener
{
public:
    explicit BleActivationStrategy(IBleChip& bleChip);
    ~BleActivationStrategy() final = default;

    void onConnectivityChangeUsb() final;
    void onConnectivityChangeBle() final;
    void onConnectivityChangeNone() final;

private:
    IBleChip& m_bleChip;
};

inline BleActivationStrategy::BleActivationStrategy(IBleChip& bleChip)
    : m_bleChip(bleChip)
{
}

inline void BleActivationStrategy::onConnectivityChangeUsb()
{
    m_bleChip.disableBle();
}

inline void BleActivationStrategy::onConnectivityChangeBle()
{
    m_bleChip.enableBle();
}

inline void BleActivationStrategy::onConnectivityChangeNone()
{
    m_bleChip.disableBle();
}