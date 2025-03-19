/*
 * CustomKeyProtocol.hpp
 *
 *  Created on: Sep 14, 2018
 *      Author: Labo
 */

#ifndef NEMEIO_CUSTOMKEYPROTOCOL_CUSTOMKEYPROTOCOL_HPP_
#define NEMEIO_CUSTOMKEYPROTOCOL_CUSTOMKEYPROTOCOL_HPP_

#include <vector>

#include "IKeyDataSender.hpp"
#include "ComputerHIDMessageHandler.h"
#include "etl/optional.h"
#include "HidSequence.hpp"

class CustomKeyProtocol
{
public:
    CustomKeyProtocol(IKeyDataSender& keyDataSender,
                      ComputerHIDMessageHandler& computerHIDMessageHandler);
    virtual ~CustomKeyProtocol() = default;

    void selectTouch(KeyIntIndex keyIdx, bool bSelect);

    void sendKeyReport(const Configuration& configuration);
    void resetKeys();

private:
    enum CommandID
    {
        COMMANDID_PRESSEDKEYS = 1
    };
    std::vector<KeyIntIndex> mSelectedKeys;
    IKeyDataSender& mKeyDataSender;
    ComputerHIDMessageHandler& mComputerHIDMessageHandler;
    etl::optional<HidSequence> m_currentSequence;

    void sendCmdPressedKeys();

    void setPressedKeysPayload(std::vector<uint8_t>& dataToSend);
    void setPressedKey(KeyIntIndex key, std::vector<uint8_t>& dataToSend);
    void updatePayloadLength(uint32_t payloadLengthIdx,
                             uint16_t payloadLength,
                             std::vector<uint8_t>& dataToSend);

    bool isWinSecureAttentionSequence(const std::vector<KeyIntIndex>& selectedKeys,
                                      const Configuration& configuration);
    bool isCapslockSequence(const std::vector<KeyIntIndex>& selectedKeys,
                            const Configuration& configuration);
    bool findSequence(const std::vector<KeyIntIndex>& sequenceToFind,
                      etl::array_view<const configuration::KeySequence> sequenceList) const;
    void sendWinSecureAttentionSequence(const Configuration& configuration);
    void sendCapslockKey(const Configuration& configuration);
};

#endif /* NEMEIO_CUSTOMKEYPROTOCOL_CUSTOMKEYPROTOCOL_HPP_ */
