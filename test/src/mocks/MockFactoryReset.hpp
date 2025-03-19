#ifndef MOCKS_MOCKFACTORYRESET_HPP_
#define MOCKS_MOCKFACTORYRESET_HPP_

#include "gmock/gmock.h"
#include "IFactoryReset.hpp"

class MockFactoryReset : public IFactoryReset
{
public:
    MOCK_METHOD(void,
                run,
                (const std::function<void(IFactoryReset::ErrCode)>& actionBeforeSystemReset),
                (override));
};

#endif /* MOCKS_MOCKFACTORYRESET_HPP_ */