/*
 * InterfaceHAL.cpp
 *
 *  Created on: 5 avr. 2017
 *      Author: cgarnier
 */

#include "AbstractHID.h"
#include "utils.hpp"

AbstractHID::AbstractHID()
    : mpKeyLockStatesListener(nullptr)
{
}

const uint16_t AbstractHID::gReportLengths[gMaxReportID] = {CONSUMER_SZ, KEYBOARD_SZ, SYSTEMCTRL_SZ};

uint16_t AbstractHID::associateLaunchReport(HIDKey::Keycode al) const
{
    uint16_t array;
    switch(al)
    {
    case HIDKey::volume_up:
        array = 0x0001;
        break;
    case HIDKey::volume_down:
        array = 0x0002;
        break;
    case HIDKey::volume_mute:
        array = 0x0004;
        break;
    case HIDKey::search:
        array = 0x0008;
        break;
    case HIDKey::brightness_inc:
        array = 0x0010;
        break;
    case HIDKey::brightness_dec:
        array = 0x0020;
        break;
    default:
        array = 0x0000;
        break;
    }
    return array;
}

uint8_t AbstractHID::associateKeycode(uint16_t key) const
{
    uint8_t array = static_cast<uint8_t>(key);

    return array;
}

uint8_t AbstractHID::associateSystemCtrl(HIDKey::Keycode keycode) const
{
    uint8_t array;
    switch(keycode)
    {
    case HIDKey::sleep:
        array = 0x01;
        break;
    default:
        array = 0x00;
        break;
    }
    return array;
}

template<std::size_t arraySize, uint8_t id>
void writeReport(ReportBuffer<arraySize + 1, id>& report,
                 const std::array<uint8_t, arraySize>& data,
                 bool bSelect)
{
}

template<std::size_t arraySize, uint8_t id, typename T>
void AbstractHID::writeReport(ReportBuffer<arraySize, id>& report, const T& value, bool bSelect)
{
    static_assert(sizeof(value) + 1 == arraySize, "Wrong value size");
    std::array<uint8_t, arraySize - 1> dataToWrite;

    std::copy_n(reinterpret_cast<const uint8_t*>(&value), dataToWrite.size(), dataToWrite.begin());

    if(bSelect)
    {
        for(size_t i = 0; i < dataToWrite.size(); ++i)
        {
            report[i + 1] |= dataToWrite[i];
        }
    }
    else
    {
        for(size_t i = 0; i < dataToWrite.size(); ++i)
        {
            report[i + 1] &= ~dataToWrite[i];
        }
    }
}

void AbstractHID::launchApp(const HIDKey::Keycode keycode, bool bSelect)
{
    uint16_t buf = associateLaunchReport(keycode);

    writeReport(mReportBufConsumer, buf, bSelect);
}

void AbstractHID::systemCtrl(const HIDKey::Keycode keycode, bool bSelect)
{
    uint8_t buf = associateSystemCtrl((HIDKey::Keycode) keycode);

    writeReport(mReportBufSystemCtrl, buf, bSelect);
}

void AbstractHID::compareKeys(
    uint8_t* pkey2, uint8_t* pkey3, uint8_t* pkey4, uint8_t* pkey5, uint8_t* pkey6)
{
    if((*pkey2 == (*pkey3)) && (*pkey3 == (*pkey4)) && (*pkey4 == (*pkey5)) && (*pkey5 == (*pkey6)))
    {
        *pkey2 = *pkey3 = *pkey4 = *pkey5 = *pkey6 = 0xFF;
    }
    else if((*pkey3 == (*pkey4)) && (*pkey4 == (*pkey5)) && (*pkey5 == (*pkey6)))
    {
        *pkey3 = *pkey4 = *pkey5 = *pkey6 = 0xFF;
    }
    else if((*pkey4 == (*pkey5)) && (*pkey5 == (*pkey6)))
    {
        *pkey4 = *pkey5 = *pkey6 = 0xFF;
    }
    else if(*pkey5 == (*pkey6))
    {
        *pkey5 = *pkey6 = 0xFF;
    }
}

void AbstractHID::setReport(uint8_t* report, uint8_t value, uint8_t pos)
{
    report[pos] = value;
}

void AbstractHID::selectTouch(const uint16_t keyIdx, bool bSelect, bool bMacLayout)
{
    if(static_cast<size_t>(keyIdx) >= mHIDKeys.size())
    {
        return;
    }

    mKeyboardContext.setMacConfig(bMacLayout);

    HIDKey& hidKey = castToBase<HIDKey>(mHIDKeys[keyIdx]);

    HIDKey::Keycode keycode = HIDKey::null;
    if(bSelect)
    {
        keycode = hidKey.pressedKey(mKeyboardContext);
    }
    else
    {
        keycode = hidKey.unpressedKey();
    }

    if(HIDKey::Fn == keycode)
    {
        mKeyboardContext.setFnKeyPressed(bSelect);
    }
    else
    {
        selectKeyCode(keycode, bSelect);
    }
}

void AbstractHID::selectKeyCode(const HIDKey::Keycode key, bool bSelect)
{
    if(HIDKey::Fn == key)
    {
        return;
    }
    else if((key >= HIDKey::ctrl_left) && (key <= HIDKey::GUI_right))
    { //start of modifier ID : 98 and end at 107;
        uint8_t buf = key;
        if(bSelect)
            mReportBufKeyboard[1] |= buf;
        else
            mReportBufKeyboard[1] &= ~buf;
    }
    else if(key > HIDKey::GUI_right && key < HIDKey::sleep)
    {
        launchApp(key, bSelect);
    }
    else if(key >= HIDKey::sleep)
    {
        systemCtrl(key, bSelect);
    }
    else
    { //search if the location in buffer is available, up to 6 keys at a time
        uint8_t i = 3;
        uint8_t buf = key;
        if(bSelect)
        {
            while((mReportBufKeyboard[i] != 0) && (i < mReportBufKeyboard.size()))
            {
                i++;
            }
            mReportBufKeyboard[i] = buf;
        }
        else
        {
            bool bFound = false;

            // find item
            do
            {
                if(mReportBufKeyboard[i] == buf)
                {
                    bFound = true;
                }
                else
                {
                    ++i;
                }
            } while(!bFound && i < mReportBufKeyboard.size());

            // recompact at the begining
            while(i < (mReportBufKeyboard.size() - 1) && mReportBufKeyboard[i] != 0)
            {
                mReportBufKeyboard[i] = mReportBufKeyboard[i + 1];
                ++i;
            }
            if(bFound)
            {
                mReportBufKeyboard[i] = 0;
            }
        }
    }
}

void AbstractHID::sendReports()
{
    sendSingleReport(mReportBufConsumer);
    sendSingleReport(mReportBufKeyboard);
    sendSingleReport(mReportBufSystemCtrl);
}

template<std::size_t arraySize, uint8_t id>
void AbstractHID::sendSingleReport(ReportBuffer<arraySize, id>& reportBuffer)
{
    reportBuffer.send([this](const std::array<uint8_t, arraySize>& reportBufferArray)
                      { sendReport(reportBufferArray.data(), reportBufferArray.size()); });
}

void AbstractHID::clearReports()
{
    clearKeys();

    mReportBufConsumer.clear();
    mReportBufKeyboard.clear();
    mReportBufSystemCtrl.clear();
}

void AbstractHID::clearKeys()
{
    mKeyboardContext.clear();
    for(auto ite = mHIDKeys.begin(); ite != mHIDKeys.end(); ++ite)
    {
        HIDKey& hidKey = castToBase<HIDKey>(*ite);
        hidKey.unpressedKey();
    }
}

void AbstractHID::registerKeyLockStatesListener(IKeyLockStatesListener& listener)
{
    mpKeyLockStatesListener = &listener;
}

void AbstractHID::notifyKeyLockStatesChanged(const HIDDefs::KeyLockStates& keyLockStates)
{
    if(mpKeyLockStatesListener != nullptr)
    {
        mpKeyLockStatesListener->keyLockStatesChanged(*this, keyLockStates);
    }
}

void AbstractHID::receivedOutputReport(const uint8_t* pData, std::size_t len)
{
    if(pData != nullptr && len > sizeof(uint8_t))
    {
        uint8_t reportId = pData[0];
        ++pData;
        --len;

        if(isKeyLockStates(reportId, len))
        {
            parseKeyLockStates(pData[0]);
        }
    }
}

bool AbstractHID::isKeyLockStates(uint8_t reportId, std::size_t len)
{
    return HIDDefs::HID_KEY_LOCK_STATES_REPORT_ID == reportId
           && HIDDefs::HID_SET_KEY_LOCK_STATES_LEN == len;
}

void AbstractHID::parseKeyLockStates(uint8_t data)
{
    HIDDefs::KeyLockStates keyLockStates(data);
    notifyKeyLockStatesChanged(keyLockStates);
}
