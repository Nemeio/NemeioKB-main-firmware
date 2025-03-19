#include "GetPressedKeysCmd.hpp"
#include "assertTools.h"

using namespace benchmark;

GetPressedKeysCmd::GetPressedKeysCmd(IAppCommTxListener& appCommTxListener,
                                     TestBenchKeypad& testBenchKeypad)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_FUNCTIONAL_GETPRESSEDKEYS, appCommTxListener)
    , m_testBenchKeypad(testBenchKeypad)
{
}

void GetPressedKeysCmd::init()
{
    if(!m_isKeypadInited)
    {
        m_testBenchKeypad.init();
        m_isKeypadInited = true;
    }
}

void GetPressedKeysCmd::deinit()
{
    if(m_isKeypadInited)
    {
        m_testBenchKeypad.deinit();

        m_isKeypadInited = false;
    }
}

void GetPressedKeysCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    Response response;
    response.retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;
    size_t sizeResponse = sizeof(response.retCode);

    if(0 == dataLen)
    {
        response.retCode = AppCommDefs::APPCOMMERR_SUCCESS;

        // To avoid interfering with IO matrix test, wait the first call to this function to init the keypad
        init();

        etl::array_view<const uint8_t> pressedKeys = m_testBenchKeypad.getPressedKeys();
        ASSERT(pressedKeys.size() < sizeof(response.pressedKeys));
        etl::copy(pressedKeys.begin(), pressedKeys.end(), response.pressedKeys);
        sizeResponse += pressedKeys.size();
    }

    transmitResponse(reinterpret_cast<uint8_t*>(&response), sizeResponse);
}