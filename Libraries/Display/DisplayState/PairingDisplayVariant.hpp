/*
 * PairingDisplayState.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_PAIRINGDISPLAYVARIANT_HPP_
#define DISPLAY_DISPLAYSTATE_PAIRINGDISPLAYVARIANT_HPP_

#include "MainDisplayVariant.hpp"
#include "BLEDefs.hpp"
#include "WriteBuffer.hpp"
#include "ScreenDefs.hpp"
#include "IScreen.hpp"

class PairingDisplayVariant : public MainDisplayVariant
{
public:
    PairingDisplayVariant();
    PairingDisplayVariant(const BLEDefs::Passkey& passkey);
    virtual ~PairingDisplayVariant() = default;

    void draw(IScreen& screen, uint8_t fbNum);

    bool operator==(const PairingDisplayVariant& variant) const;

    void pairingStart(const BLEDefs::Passkey& passkey);
    void pairingStop();

private:
    static constexpr std::size_t KEY_HEIGHT = 48;

    static constexpr std::size_t KEY_DIGITS_WIDTH = 32;
    static constexpr std::size_t KEY_DIGITS_X_START = 2400 - (645 + 39);
    static constexpr std::size_t KEY_DIGITS_X_SPACE = -170;
    static constexpr std::size_t KEY_DIGITS_Y = 555 + 31;

    static constexpr std::size_t KEY_RSP_WIDTH = 48;
    static constexpr std::size_t KEY_OK_X = 2400 - 2160 - 110;
    static constexpr std::size_t KEY_OK_Y = 555 + 31;
    static constexpr std::size_t KEY_NOK_X = 2400 - 2220 - 78;
    static constexpr std::size_t KEY_NOK_Y = 220 + 31;

    static constexpr std::size_t KEY_DIGITS_BUFFER_SIZE = KEY_HEIGHT * KEY_DIGITS_WIDTH / 8;
    static constexpr std::size_t KEY_RSP_BUFFER_SIZE = KEY_HEIGHT * KEY_RSP_WIDTH / 8;

    static constexpr std::size_t NB_DIGITS = 10;
    static constexpr std::size_t NB_KEYS = BLEDefs::PASSKEY_SIZE + 2;

    static constexpr Image::eImageType DIGITS_IMAGE_TYPE = Image::eImageType::_1BPP;
    static constexpr uint8_t DIGITS_FRAME_BUFFER = 0;

    static const uint8_t keyNum[NB_DIGITS][KEY_DIGITS_BUFFER_SIZE];
    static const uint8_t keyOK[KEY_RSP_BUFFER_SIZE];
    static const uint8_t keyNOK[KEY_RSP_BUFFER_SIZE];
    bool displayPasskey(IScreen& screen, const BLEDefs::Passkey& passkey, uint8_t fbNum);

    bool fillDigitsInFramebuffer(IFrameBuffer& frameBuffer, const BLEDefs::Passkey& passkey);
    bool fillOkButtonInFramebuffer(IFrameBuffer& frameBuffer);
    bool fillNokButtonInFramebuffer(IFrameBuffer& frameBuffer);

    BLEDefs::Passkey mPasskey;
};

#endif /* DISPLAY_DISPLAYSTATE_PAIRINGDISPLAYVARIANT_HPP_ */
