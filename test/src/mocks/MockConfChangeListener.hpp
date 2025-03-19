#ifndef MOCKS_MOCKCONFCHANGELISTENER_HPP_
#define MOCKS_MOCKCONFCHANGELISTENER_HPP_

#include "gmock/gmock.h"
#include "IConfChangeListener.hpp"

class MockConfChangeListener : public IConfChangeListener
{
public:
    MOCK_METHOD(void, configurationChanged, (const Configuration* config), (override));
};

#endif /* MOCKS_MOCKCONFCHANGELISTENER_HPP_ */