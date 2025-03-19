#ifndef MOCKS_MOCKKEYPROTOCOLLISTENER_HPP_
#define MOCKS_MOCKKEYPROTOCOLLISTENER_HPP_

#include "gmock/gmock.h"
#include "IKeyProtocolListener.hpp"

class MockKeyProtocolListener : public IKeyProtocolListener
{
public:
    MOCK_METHOD(void,
                keyProtocolModeChanged,
                (keyprotocoldefs::KeyProtocolMode keyProtocolMode),
                (override));
};

#endif /* MOCKS_MOCKKEYPROTOCOLLISTENER_HPP_ */