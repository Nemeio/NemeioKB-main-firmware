#pragma once

#include "BLESPICommands.hpp"
#include "etl/vector.h"
#include "etl/array_view.h"
#include "assertTools.h"

class BleCmdPacket
{
public:
    using Parameter = etl::array_view<const uint8_t>;
    using ParameterList = etl::vector<uint8_t, BLESPICommands::MAX_PARAMETERS_SIZE>;

    BLESPICommands::CommandId getId() const { return m_id; }
    void setId(BLESPICommands::CommandId id) { m_id = id; }

    const ParameterList& getParameters() const { return m_parameters; }
    void addParameter(const Parameter& parameter)
    {
        size_t length = parameter.size();

        ASSERT(length + sizeof(uint16_t) <= m_parameters.available());

        // Send parameter header (i.e. parameter length)
        m_parameters.push_back(reinterpret_cast<uint8_t*>(&length)[1]);
        m_parameters.push_back(reinterpret_cast<uint8_t*>(&length)[0]);

        m_parameters.insert(m_parameters.end(), parameter.begin(), parameter.end());
    }

private:
    BLESPICommands::CommandId m_id;
    ParameterList m_parameters;
};