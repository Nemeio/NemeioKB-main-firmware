#ifndef MOCKS_MOCKCONFIGURATIONPARSER_HPP_
#define MOCKS_MOCKCONFIGURATIONPARSER_HPP_

#include "gmock/gmock.h"
#include "IConfigurationParser.hpp"

class MockConfigurationParser : public IConfigurationParser
{
public:
    MOCK_METHOD(Configuration, parse, (const uint8_t* data), (override));
    MOCK_METHOD(void,
                toFormat,
                (const Configuration& configuration, embedded_ostream& oss),
                (override));
};

#endif /* MOCKS_MOCKCONFIGURATIONPARSER_HPP_ */