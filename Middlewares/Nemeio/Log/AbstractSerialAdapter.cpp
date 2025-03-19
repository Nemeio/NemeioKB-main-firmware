#include "AbstractSerialAdapter.hpp"

AbstractSerialAdapter& AbstractSerialAdapter::operator<<(etl::string_view data)
{
    writeBuffer(reinterpret_cast<const uint8_t*>(data.data()), data.size());

    return (*this);
}