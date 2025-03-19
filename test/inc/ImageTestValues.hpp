#ifndef IMAGE_TEST_VALUES
#define IMAGE_TEST_VALUES

#include <cstdint>

namespace TestImage
{
inline constexpr std::size_t WIDTH = 1496;
inline constexpr std::size_t HEIGHT = 624;
inline constexpr std::size_t SIZE = WIDTH * HEIGHT;
inline constexpr std::size_t SIZE_1BPP = SIZE / 8;
inline constexpr std::size_t SIZE_2BPP = SIZE / 4;
} // namespace TestImage

#endif //IMAGE_VALUES
