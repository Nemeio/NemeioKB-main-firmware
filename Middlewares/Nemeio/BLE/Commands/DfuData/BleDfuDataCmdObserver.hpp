#include "Observer.hpp"
#include "BLEDefs.hpp"

inline constexpr size_t DFU_PACKET_MAX_SIZE = 64;
inline constexpr size_t SLIP_DATA_MAX_SIZE = DFU_PACKET_MAX_SIZE * 2 + 1;

struct BleDfuResponse
{
    const uint8_t* data;
    size_t len;
};

using BleDfuDataCmdObserver = etl::observer<BleDfuResponse>;