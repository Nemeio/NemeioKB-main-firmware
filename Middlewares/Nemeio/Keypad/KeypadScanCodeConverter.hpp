#pragma once

#include "IKeypadScanCodeConverter.hpp"
#include "iNemeioKeypadPressedListener.h"

class KeypadScanCodeConverter : public IKeypadScanCodeConverter
{
public:
    KeypadScanCodeConverter();
    ~KeypadScanCodeConverter() final = default;

    void convertScanCodeToKeyUpdate(etl::array_view<const uint16_t> scanCodes) final;
    void registerKeyListener(iNemeioKeypadPressedListener& iKeypadListener);
    void unregisterKeyListener(iNemeioKeypadPressedListener& iKeypadListener);

private:
    static constexpr size_t NB_MAX_LISTENERS = 4;

    struct KeyState
    {
        ButtonID button;
        bool isPressed;
    };

    etl::array<uint16_t, KeypadDefs::MAX_ROW> mapping;
    etl::vector<iNemeioKeypadPressedListener*, NB_MAX_LISTENERS> m_keypadListeners;

    uint8_t getNbKeyPressedInRow(uint16_t rowScanCodes) const;
    bool detectGhostingRow(size_t idx, etl::array_view<const uint16_t> scanCodes) const;
    void updateKeyState(bool pressed, ButtonID button);
};