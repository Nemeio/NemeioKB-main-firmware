#ifndef MOCKS_MOCKWALLPAPERUPDATESTRATEGY_HPP_
#define MOCKS_MOCKWALLPAPERUPDATESTRATEGY_HPP_

#include "gmock/gmock.h"
#include "IWallpaperUpdateStrategy.hpp"

class MockWallpaperUpdateStrategy : public IWallpaperUpdateStrategy
{
public:
    MOCK_METHOD(uint8_t, checkConfiguration, (std::size_t len), (override));
    MOCK_METHOD(uint8_t, terminateConfigurationUpdate, (), (override));
};

#endif /* MOCKS_MOCKWALLPAPERUPDATESTRATEGY_HPP_ */
