/*
 * AbstractHID.h
 *
 *  Created on: 5 avr. 2017
 *      Author: cgarnier
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ABSTRACTHID_H_
#define ABSTRACTHID_H_

/* Includes ------------------------------------------------------------------*/
#include <string>
#include "etl/variant.h"
#include "etl/array.h"
#include <cstdint>
#include <cstdlib>
#include <cstdbool>
#include "FnHIDKey.hpp"
#include "KeyLockStates/IKeyLockStatesListener.hpp"
#include "HIDDefs.hpp"
#include "HIDKey.hpp"
#include "Identifiable.hpp"
#include "MacHIDKey.hpp"
#include "ReportBuffer.hpp"

class AbstractHID : public Identifiable
{
public:
    AbstractHID();
    virtual ~AbstractHID() = default;

    //used with the new version

    /*
	 * Launch application from the enum list Abstract_AppLaunch
	 * USBD_HandleTypeDef *pdev : pointer to the hid device
	 * Abstract_AppLaunch al : key of the application to launch
	 */
    void selectTouch(const uint16_t keyIdx, bool bSelect, bool bMacLayout);
    void selectKeyCode(const HIDKey::Keycode key, bool bSelect);

    void sendReports();
    void clearReports();

    void registerKeyLockStatesListener(IKeyLockStatesListener& listener);

private:
    etl::array<etl::variant<HIDKey, FnHIDKey, MacHIDKey>, 80> mHIDKeys = {
        HIDKey(HIDKey::escape),
        FnHIDKey(HIDKey::F1, HIDKey::volume_mute),
        FnHIDKey(HIDKey::F2, HIDKey::volume_down),
        FnHIDKey(HIDKey::F3, HIDKey::volume_up),
        HIDKey(HIDKey::F4),
        HIDKey(HIDKey::F5),
        HIDKey(HIDKey::F6),
        HIDKey(HIDKey::F7),
        HIDKey(HIDKey::F8),
        FnHIDKey(HIDKey::F9, HIDKey::search),
        HIDKey(HIDKey::F10),
        FnHIDKey(HIDKey::F11, HIDKey::brightness_dec),
        FnHIDKey(HIDKey::F12, HIDKey::brightness_inc),
        HIDKey(HIDKey::insert),
        HIDKey(HIDKey::delete_forward),

        HIDKey(HIDKey::grave_accent),
        HIDKey(HIDKey::b_1),
        HIDKey(HIDKey::b_2),
        HIDKey(HIDKey::b_3),
        HIDKey(HIDKey::b_4),
        HIDKey(HIDKey::b_5),
        HIDKey(HIDKey::b_6),
        HIDKey(HIDKey::b_7),
        HIDKey(HIDKey::b_8),
        HIDKey(HIDKey::b_9),
        HIDKey(HIDKey::b_0),
        HIDKey(HIDKey::minus),
        HIDKey(HIDKey::equal),
        HIDKey(HIDKey::delete_bksp),

        HIDKey(HIDKey::tab),
        HIDKey(HIDKey::q),
        HIDKey(HIDKey::w),
        HIDKey(HIDKey::e),
        HIDKey(HIDKey::r),
        HIDKey(HIDKey::t),
        HIDKey(HIDKey::y),
        HIDKey(HIDKey::u),
        HIDKey(HIDKey::i),
        HIDKey(HIDKey::o),
        HIDKey(HIDKey::p),
        HIDKey(HIDKey::bracket_in),
        HIDKey(HIDKey::bracket_out),
        HIDKey(HIDKey::apostrophe),

        HIDKey(HIDKey::caps_lock),
        HIDKey(HIDKey::a),
        HIDKey(HIDKey::s),
        HIDKey(HIDKey::d),
        HIDKey(HIDKey::f),
        HIDKey(HIDKey::g),
        HIDKey(HIDKey::h),
        HIDKey(HIDKey::j),
        HIDKey(HIDKey::k),
        HIDKey(HIDKey::l),
        HIDKey(HIDKey::semicolon),
        HIDKey(HIDKey::non_us_sharp),
        HIDKey(HIDKey::enter),

        HIDKey(HIDKey::shift_left),
        HIDKey(HIDKey::non_us_anti_slash),
        HIDKey(HIDKey::z),
        HIDKey(HIDKey::x),
        HIDKey(HIDKey::c),
        HIDKey(HIDKey::v),
        HIDKey(HIDKey::b),
        HIDKey(HIDKey::n),
        HIDKey(HIDKey::m),
        HIDKey(HIDKey::comma),
        HIDKey(HIDKey::dot),
        HIDKey(HIDKey::slash),
        HIDKey(HIDKey::up_arrow),
        HIDKey(HIDKey::shift_right),

        MacHIDKey(HIDKey::ctrl_left, HIDKey::Fn),
        MacHIDKey(HIDKey::Fn, HIDKey::ctrl_left),
        MacHIDKey(HIDKey::GUI_left, HIDKey::alt_left),
        MacHIDKey(HIDKey::alt_left, HIDKey::GUI_left),
        HIDKey(HIDKey::spacebar),
        MacHIDKey(HIDKey::alt_right, HIDKey::GUI_right),
        MacHIDKey(HIDKey::ctrl_right, HIDKey::alt_right),
        FnHIDKey(HIDKey::left_arrow, HIDKey::home_kc),
        HIDKey(HIDKey::down_arrow),
        FnHIDKey(HIDKey::right_arrow, HIDKey::end),
    };

    static constexpr uint8_t CONSUMER_ID = 1;
    static constexpr uint8_t KEYBOARD_ID = 2;
    static constexpr uint8_t SYSTEMCTRL_ID = 3;

    static constexpr std::size_t CONSUMER_SZ = 2;
    static constexpr std::size_t KEYBOARD_SZ = 8;
    static constexpr std::size_t SYSTEMCTRL_SZ = 1;


    ReportBuffer<CONSUMER_SZ + 1, CONSUMER_ID> mReportBufConsumer;
    ReportBuffer<KEYBOARD_SZ + 1, KEYBOARD_ID> mReportBufKeyboard;
    ReportBuffer<SYSTEMCTRL_SZ + 1, SYSTEMCTRL_ID> mReportBufSystemCtrl;

    KeyboardContext mKeyboardContext;

    IKeyLockStatesListener* mpKeyLockStatesListener;

protected:
    static constexpr uint8_t gMaxReportID = SYSTEMCTRL_ID;
    static const uint16_t gReportLengths[gMaxReportID];

    /*
	 * Unspecified keys return 0 when the method "sendTouch" is used.
	 * compareKeys compare each keys (given and ungiven ones) to each other to be sure
	 * that there are not the same key following multiple times
	 * uint8_t *pkeys : pointer to the provided key
	 */
    void compareKeys(uint8_t* pkey2, uint8_t* pkey3, uint8_t* pkey4, uint8_t* pkey5, uint8_t* pkey6);

    template<std::size_t arraySize, uint8_t id, typename T>
    void writeReport(ReportBuffer<arraySize, id>& report, const T& value, bool bSelect);

    void launchApp(const HIDKey::Keycode keycode, bool bSelect);
    void systemCtrl(const HIDKey::Keycode keycode, bool bSelect);

    /*
	 * Associate the launch report corresponding to the application
	 * al : enum value of the application needed
	 * return the hexadecimal value of the application launch key
	 */
    [[nodiscard]] uint16_t associateLaunchReport(HIDKey::Keycode al) const;
    /*
	 * Associate the keycode to the key
	 * key : enum equivalent of the keycode (hexadecimal value of the key)
	 * return the hexadecimal value of the key
	 */
    [[nodiscard]] uint8_t associateKeycode(uint16_t key) const;

    [[nodiscard]] uint8_t associateSystemCtrl(HIDKey::Keycode keycode) const;

    void setReport(uint8_t* report, uint8_t value, uint8_t pos);

    void clearKeys();

    template<std::size_t arraySize, uint8_t id>
    void sendSingleReport(ReportBuffer<arraySize, id>& reportBuffer);

    virtual void sendReport(const uint8_t* reportBuf, size_t reportBufSize) = 0;

    void notifyKeyLockStatesChanged(const HIDDefs::KeyLockStates& keyLockStates);

    void receivedOutputReport(const uint8_t* pData, std::size_t len);
    bool isKeyLockStates(uint8_t reportId, std::size_t len);
    void parseKeyLockStates(uint8_t data);
};
#endif /* ABSTRACTHID_H_ */
