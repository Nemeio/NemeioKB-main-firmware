#pragma once

#include "AppCommCmd.hpp"
#include "TestBenchKeypad.hpp"
#include "utils.hpp"

namespace benchmark
{
class GetPressedKeysCmd : public AppCommCmd
{
public:
    GetPressedKeysCmd(IAppCommTxListener& appCommTxListener, TestBenchKeypad& testBenchKeypad);
    ~GetPressedKeysCmd() final = default;

    void deinit();

private:
    bool m_isKeypadInited{false};
    TestBenchKeypad& m_testBenchKeypad;

    PACK((struct Response
          {
              AppCommDefs::AppCommErrCode retCode;
              uint8_t pressedKeys[TestBenchKeypad::MAX_PRESSED_KEYS];
          }));

    void dataReceived(const uint8_t* pData, size_t dataLen) final;

    // The init function is private because it will interfere with IO matrix test
    void init();
};

} // namespace benchmark
