/*
 * BLEPairing.hpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_BLE_BLEPAIRING_BLEPAIRING_HPP_
#define NEMEIO_BLE_BLEPAIRING_BLEPAIRING_HPP_

#include <iterator>
#include "IBLEPairingListener.hpp"
#include "BLEDefs.hpp"
#include "IBleChip.hpp"
#include "IKeyMessageHandler.hpp"
#include "ListenerCollection.hpp"
#include "BleNumericComparisonCmd.hpp"
#include "BlePairingAbortCmd.hpp"
#include "cpp_helper.hpp"

template<std::size_t N>
class BLEPairing : public BleNumericComparisonCmdObserver,
                   public BlePairingAbortCmdObserver,
                   public IKeyMessageHandler
{
public:
    BLEPairing(std::array<IBLEPairingListener*, N> listeners, IBleChip& bleChip);
    virtual ~BLEPairing();
    COPYABLE_MOVABLE(BLEPairing, delete);

    void replyPairing(bool bAccept);

    void pressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void unpressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void terminateUpdateKeysWithConfig(const Configuration& config) final;
    void resetKeysWithConfig(const Configuration& config) final;

    void notification(const BleNumericComparisonEvent& event) final;
    void notification(BlePairingAbortEvent event) final;

private:
    static const ButtonID BUTTON_NUMERIC_COMPARISON_OK;
    static const ButtonID BUTTON_NUMERIC_COMPARISON_NOK;

    static constexpr UBaseType_t TIMEOUT_TASK_PRIORITY = 3;
    static constexpr configSTACK_DEPTH_TYPE TIMEOUT_TASK_STACK_SIZE = 256;
    static constexpr TickType_t PAIRING_TIMEOUT = BLEDefs::TIMEOUT_SECONDS + 1000;
    static constexpr TickType_t TIMER_WAIT_DELAY = 10;

    typedef uint8_t PairingAbortDummyParm;

    bool mPairingInProgress;
    ListenerCollection<IBLEPairingListener*, N> mListeners;
    BleNumericComparisonCmd mNumericComparisonCmd;
    BlePairingAbortCmd mPairingAbortCmd;
    IBleChip& mBLEChip;
    Signal mTimeoutSignal;
    TaskHandle_t mPairingTimeoutTaskHandle;
    TimerHandle_t mPairingTimeoutTimer;

    void startPairing(const BLEDefs::Passkey& passkey);
    void notifyPairingStart(const BLEDefs::Passkey& passkey);

    void stopPairing();
    void notifyPairingStop();

    void startTimeoutTimer();
    void stopTimeoutTimer();

    static void pairingTimeoutTimerCallback(TimerHandle_t xTimer);
    static void vPairingTimeoutTask(void* pvParameters);
};

#include "BLEPairing.tpp"

#endif /* NEMEIO_BLE_BLEPAIRING_BLEPAIRING_HPP_ */
