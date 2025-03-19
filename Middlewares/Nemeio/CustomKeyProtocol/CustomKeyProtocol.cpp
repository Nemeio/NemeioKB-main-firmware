/*
 * CustomKeyProtocol.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: Labo
 */

#include "CustomKeyProtocol.hpp"

#include <cstdint>
#include <algorithm>

#ifdef TEST_SAS
static const char CTRL[] = "Ctrl";
static const char ALT[] = "Alt";
static const char DELETE[] = "Del";
static const char SHIFT[] = "Shift";
static const char ALTGR[] = "AltGr";
static const char A_CHAR[] = "A";
#endif

CustomKeyProtocol::CustomKeyProtocol(IKeyDataSender& keyDataSender,
                                     ComputerHIDMessageHandler& computerHIDMessageHandler)
    : mKeyDataSender(keyDataSender)
    , mComputerHIDMessageHandler(computerHIDMessageHandler)
{
}

void CustomKeyProtocol::selectTouch(KeyIntIndex keyIdx, bool bSelect)
{
    auto ite = std::find(mSelectedKeys.begin(), mSelectedKeys.end(), keyIdx);
    bool bFound = (ite != mSelectedKeys.end());

    if(bSelect && !bFound)
    {
        mSelectedKeys.push_back(keyIdx);
    }
    else if(!bSelect && bFound)
    {
        mSelectedKeys.erase(ite);
    }
}

void CustomKeyProtocol::sendKeyReport(const Configuration& configuration)
{
    if(m_currentSequence.has_value())
    {
        m_currentSequence.value().clear(configuration);
        m_currentSequence.reset();
    }

    if(isWinSecureAttentionSequence(mSelectedKeys, configuration))
    {
        sendWinSecureAttentionSequence(configuration);
    }
    else if(isCapslockSequence(mSelectedKeys, configuration))
    {
        sendCapslockKey(configuration);
    }
    else
    {
        sendCmdPressedKeys();
    }
}

void CustomKeyProtocol::resetKeys()
{
    mSelectedKeys.clear();
}

void CustomKeyProtocol::sendCmdPressedKeys()
{
    std::vector<uint8_t> dataToSend;

    setPressedKeysPayload(dataToSend);

    mKeyDataSender.sendKeyData(dataToSend.data(), dataToSend.size());
}

void CustomKeyProtocol::setPressedKeysPayload(std::vector<uint8_t>& dataToSend)
{
    if(mSelectedKeys.size() > UINT8_MAX)
    {
        return;
    }

    dataToSend.push_back(mSelectedKeys.size());
    for(auto ite = mSelectedKeys.begin(); ite != mSelectedKeys.end(); ite++)
    {
        setPressedKey(*ite, dataToSend);
    }
}

void CustomKeyProtocol::setPressedKey(KeyIntIndex keyIdx, std::vector<uint8_t>& dataToSend)
{
    dataToSend.push_back((uint8_t) (keyIdx >> 24));
    dataToSend.push_back((uint8_t) (keyIdx >> 16));
    dataToSend.push_back((uint8_t) (keyIdx >> 8));
    dataToSend.push_back((uint8_t) (keyIdx >> 0));
}

void CustomKeyProtocol::updatePayloadLength(uint32_t payloadLengthIdx,
                                            uint16_t payloadLength,
                                            std::vector<uint8_t>& dataToSend)
{
    dataToSend[payloadLengthIdx] = (uint8_t) ((payloadLength & 0xFF00) >> 8);
    dataToSend[payloadLengthIdx + 1] = (uint8_t) (payloadLength & 0x00FF);
}

bool CustomKeyProtocol::isWinSecureAttentionSequence(const std::vector<KeyIntIndex>& selectedKeys,
                                                     const Configuration& configuration)
{
    return findSequence(selectedKeys, configuration.getWinSasSequence());
}

bool CustomKeyProtocol::isCapslockSequence(const std::vector<KeyIntIndex>& selectedKeys,
                                           const Configuration& configuration)
{
    return findSequence(selectedKeys, configuration.getCapslockKeySequences());
}

bool CustomKeyProtocol::findSequence(
    const std::vector<KeyIntIndex>& sequenceToFind,
    etl::array_view<const configuration::KeySequence> sequenceList) const
{
    etl::array_view<const KeyIntIndex> sequenceToFindView{sequenceToFind.begin(),
                                                          sequenceToFind.end()};

    auto ite = std::find_if(sequenceList.begin(),
                            sequenceList.end(),
                            [sequenceToFindView](const configuration::KeySequence& sequence)
                            { return sequence == sequenceToFindView; });

    return ite != sequenceList.end();
}

void CustomKeyProtocol::sendWinSecureAttentionSequence(const Configuration& configuration)
{
    etl::array<HIDKey::Keycode, 3> winSAS = {HIDKey::Keycode::ctrl_left,
                                             HIDKey::Keycode::alt_left,
                                             HIDKey::Keycode::delete_forward};
    m_currentSequence.emplace(winSAS, mComputerHIDMessageHandler);
    m_currentSequence.value().send(configuration);
}

void CustomKeyProtocol::sendCapslockKey(const Configuration& configuration)
{
    etl::array<HIDKey::Keycode, 1> capslock = {HIDKey::Keycode::caps_lock};

    m_currentSequence.emplace(capslock, mComputerHIDMessageHandler);
    m_currentSequence.value().send(configuration);
}