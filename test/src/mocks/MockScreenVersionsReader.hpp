#ifndef MOCKS_MOCKSCREENVERSIONSREADER_HPP_
#define MOCKS_MOCKSCREENVERSIONSREADER_HPP_

#include "gmock/gmock.h"
#include "IScreenVersionsReader.hpp"

class MockScreenVersionsReader : public IScreenVersionsReader
{
public:
    MOCK_METHOD(ScreenDefs::ScreenErrCode,
                getVersions,
                (char* firmwareVersionBuffer,
                 std::size_t firmwareVersionBufferLen,
                 char* lutVersionBuffer,
                 std::size_t lutVersionBufferLen),
                (const override));
};

#endif /* MOCKS_MOCKSCREENVERSIONSREADER_HPP_ */
