#ifndef MOCKS_MOCKKEYMESSAGEHANDLER_HPP_
#define MOCKS_MOCKKEYMESSAGEHANDLER_HPP_

#include "gmock/gmock.h"
#include "IKeyMessageHandler.hpp"

class MockKeyMessageHandler : public IKeyMessageHandler
{
public:
    MOCK_METHOD(void,
                pressedKeyWithConfig,
                (const ButtonID& button, const Configuration& config),
                (override));
    MOCK_METHOD(void,
                unpressedKeyWithConfig,
                (const ButtonID& button, const Configuration& config),
                (override));
    MOCK_METHOD(void, terminateUpdateKeysWithConfig, (const Configuration& config), (override));
    MOCK_METHOD(void, resetKeysWithConfig, (const Configuration& config), (override));
};

#endif /* MOCKS_MOCKKEYMESSAGEHANDLER_HPP_ */