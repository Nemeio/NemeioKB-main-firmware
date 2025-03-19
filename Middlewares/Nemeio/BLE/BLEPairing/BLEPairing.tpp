/*

 * BLEPairing.tpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Labo
 */

#ifndef BLE_BLEPAIRING_BLEPAIRING_TPP_
#define BLE_BLEPAIRING_BLEPAIRING_TPP_

#include "BLEPairing.hpp"

template<std::size_t N>
const ButtonID BLEPairing<N>::BUTTON_NUMERIC_COMPARISON_OK = BTN_ENTER;

template<std::size_t N>
const ButtonID BLEPairing<N>::BUTTON_NUMERIC_COMPARISON_NOK = BTN_BACKSPACE;

template<std::size_t N>
BLEPairing<N>::BLEPairing(std::array<IBLEPairingListener*, N> listeners, IBleChip& bleChip)
    : mPairingInProgress(false)
    , mListeners(listeners)
    , mBLEChip(bleChip)
{
    mNumericComparisonCmd.add_observer(*this);
    mPairingAbortCmd.add_observer(*this);
    mBLEChip.registerBleCommandListener(mPairingAbortCmd);
    mBLEChip.registerBleCommandListener(mNumericComparisonCmd);

    mPairingTimeoutTimer = xTimerCreate("pairingTimeoutTimer",
                                        PAIRING_TIMEOUT,
                                        pdFALSE,
                                        this,
                                        pairingTimeoutTimerCallback);
    BaseType_t ret = xTaskCreate(vPairingTimeoutTask,
                                 "PairingTimeoutTask",
                                 TIMEOUT_TASK_STACK_SIZE,
                                 this,
                                 TIMEOUT_TASK_PRIORITY,
                                 &mPairingTimeoutTaskHandle);
    ASSERT(pdPASS == ret);
}

template<std::size_t N>
BLEPairing<N>::~BLEPairing()
{
    mBLEChip.unregisterBleCommandListener(mPairingAbortCmd);
    mBLEChip.unregisterBleCommandListener(mNumericComparisonCmd);
}

template<std::size_t N>
void BLEPairing<N>::notification(const BleNumericComparisonEvent& event)
{
    startPairing(event.passkey);
}


template<std::size_t N>
void BLEPairing<N>::notification(BlePairingAbortEvent event)
{
    stopPairing();
}

template<std::size_t N>
void BLEPairing<N>::pressedKeyWithConfig(const ButtonID& button, const Configuration& /*config*/)
{
    if(BUTTON_NUMERIC_COMPARISON_OK == button || BUTTON_NUMERIC_COMPARISON_NOK == button)
    {
        bool bAccept = (BUTTON_NUMERIC_COMPARISON_OK == button);
        replyPairing(bAccept);
    }
}

template<std::size_t N>
void BLEPairing<N>::unpressedKeyWithConfig(const ButtonID& button, const Configuration& /*config*/)
{
    // Nothing to do
}


template<std::size_t N>
void BLEPairing<N>::terminateUpdateKeysWithConfig(const Configuration& /*config*/)
{
    // Nothing to do
}

template<std::size_t N>
void BLEPairing<N>::resetKeysWithConfig(const Configuration& /*config*/)
{
    // Nothing to do
}

template<std::size_t N>
void BLEPairing<N>::startPairing(const BLEDefs::Passkey& passkey)
{
    if(!mPairingInProgress)
    {
        notifyPairingStart(passkey);
        startTimeoutTimer();
        mPairingInProgress = true;
    }
}

template<std::size_t N>
inline void BLEPairing<N>::notifyPairingStart(const BLEDefs::Passkey& passkey)
{
    mListeners.notify(
        [passkey](IBLEPairingListener* listener)
        {
            if(listener != nullptr)
            {
                listener->pairingStart(passkey);
            }
        });
}

template<std::size_t N>
inline void BLEPairing<N>::replyPairing(bool bAccept)
{
    stopPairing();

    mNumericComparisonCmd.setAccept(bAccept);
    mBLEChip.sendCommand(BleReadyForBleTxCommandRef(etl::reference_wrapper(mNumericComparisonCmd)));
}

template<std::size_t N>
void BLEPairing<N>::stopPairing()
{
    if(mPairingInProgress)
    {
        stopTimeoutTimer();
        notifyPairingStop();
        mPairingInProgress = false;
    }
}

template<std::size_t N>
void BLEPairing<N>::notifyPairingStop()
{
    mListeners.notify(
        [](IBLEPairingListener* listener)
        {
            if(listener != nullptr)
            {
                listener->pairingStop();
            }
        });
}

template<std::size_t N>
void BLEPairing<N>::pairingTimeoutTimerCallback(TimerHandle_t xTimer)
{
    BLEPairing* pBLEPairing = reinterpret_cast<BLEPairing*>(pvTimerGetTimerID(xTimer));
    ASSERT(pBLEPairing != nullptr);

    pBLEPairing->mTimeoutSignal.set();
}

template<std::size_t N>
inline void BLEPairing<N>::startTimeoutTimer()
{
    if(xTimerStart(mPairingTimeoutTimer, TIMER_WAIT_DELAY) != pdPASS)
    {
        ASSERT(false);
    }
}

template<std::size_t N>
void BLEPairing<N>::stopTimeoutTimer()
{
    if(xTimerStop(mPairingTimeoutTimer, TIMER_WAIT_DELAY) != pdPASS)
    {
        ASSERT(false);
    }
}

template<std::size_t N>
void BLEPairing<N>::vPairingTimeoutTask(void* pvParameters)
{
    BLEPairing* pBLEPairing = reinterpret_cast<BLEPairing*>(pvParameters);
    ASSERT(pBLEPairing != nullptr);

    TickType_t delay = portMAX_DELAY;
    for(;;)
    {
        if(pBLEPairing->mTimeoutSignal.wait(delay))
        {
            pBLEPairing->stopPairing();
        }
    }
}

#endif // BLE_BLEPAIRING_BLEPAIRING_TPP_
