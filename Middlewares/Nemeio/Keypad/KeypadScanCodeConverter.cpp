#include "KeypadScanCodeConverter.hpp"
#include "assertTools.h"

KeypadScanCodeConverter::KeypadScanCodeConverter()
{
    mapping.fill(0xFFFF);
}

void KeypadScanCodeConverter::convertScanCodeToKeyUpdate(etl::array_view<const uint16_t> scanCodes)
{
    etl::vector<uint8_t, KeypadDefs::MAX_ROW> changedRows;

    /* Iterate over the scan_codes received to identify the change. Ignore all changes if ghosting is detected. */
    for(const auto& scanCode: scanCodes)
    {
        /* Check if any key state changed in this row*/
        if(uint8_t index = static_cast<uint8_t>(etl::distance(scanCodes.begin(), &scanCode));
           scanCode ^ mapping[index])
        {
            if(detectGhostingRow(index, scanCodes))
            {
                changedRows.clear();
                break;
            }
            changedRows.push_back(index);
        }
    }

    for(auto index: changedRows)
    {
        uint16_t scanCode = scanCodes[index];

        /* Check every bit on the current row */
        for(uint8_t bit = 0; bit < sizeof(uint16_t) * 8; bit++)
        {
            uint16_t current_state = (scanCode >> bit) & 0x01;

            /* If this key state changed toggle the corresponding bit in mapping and update the key state to the listeners */
            if(uint16_t old_state = (mapping[index] >> bit) & 0x01; current_state != old_state)
            {
                ButtonID button = {bit, index};
                bool isPressed = (current_state == 0);
                updateKeyState(isPressed, button);
            }
        }
    }

    /*
	* If at least one change occured since all cache mapping is updated , notify the listeners that
	* the scanning - update routine is over to send the reports
	*/
    if(!changedRows.empty())
    {
        for(auto listener: m_keypadListeners)
            listener->terminateUpdateKeys();
    }
}

void KeypadScanCodeConverter::registerKeyListener(iNemeioKeypadPressedListener& iKeypadListener)
{
    if(auto ite = std::find(m_keypadListeners.begin(), m_keypadListeners.end(), &iKeypadListener);
       m_keypadListeners.end() == ite)
    {
        m_keypadListeners.push_back(&iKeypadListener);
    }
}

void KeypadScanCodeConverter::unregisterKeyListener(iNemeioKeypadPressedListener& iKeypadListener)
{
    if(auto ite = std::find(m_keypadListeners.begin(), m_keypadListeners.end(), &iKeypadListener);
       m_keypadListeners.end() != ite)
    {
        m_keypadListeners.erase(ite);
    }
}

uint8_t KeypadScanCodeConverter::getNbKeyPressedInRow(uint16_t rowScanCodes) const
{
    return static_cast<uint8_t>(__builtin_popcount(static_cast<uint16_t>(~rowScanCodes)));
}

bool KeypadScanCodeConverter::detectGhostingRow(size_t idx,
                                                etl::array_view<const uint16_t> scanCodes) const
{
    ASSERT(scanCodes.size() >= KeypadDefs::MAX_ROW_HW_MATRIX);

    if(idx >= KeypadDefs::MAX_ROW_HW_MATRIX || getNbKeyPressedInRow(scanCodes[idx]) < 2)
    {
        return false;
    }

    bool ghostingDetected = false;

    for(size_t i = 0; i < KeypadDefs::MAX_ROW_HW_MATRIX; ++i)
    {
        if(idx != i && (static_cast<uint16_t>(~scanCodes[idx] & ~scanCodes[i]) != 0))
        {
            ghostingDetected = true;
            break;
        }
    }

    return ghostingDetected;
}

void KeypadScanCodeConverter::updateKeyState(bool isPressed, ButtonID button)
{
    mapping[button.y] ^= (1UL << button.x);

    if(isPressed)
    {
        for(auto listener: m_keypadListeners)
            listener->pressedKey(button);
    }
    else
    {
        for(auto listener: m_keypadListeners)
            listener->unpressedKey(button);
    }
}