/*
 * TechnicalErrorDefs.hpp
 *
 *  Created on: Feb 25, 2020
 *      Author: Labo
 */

#ifndef NEMEIO_TECHNICALERROR_TECHNICALERRORDEFS_HPP_
#define NEMEIO_TECHNICALERROR_TECHNICALERRORDEFS_HPP_

#include "Identification.hpp"
#include "embedded_ostream.hpp"

namespace TechnicalErrorDefs
{
typedef uint16_t ErrorCode;

struct TechnicalError
{
    Identification::ComponentId component{Identification::ComponentId::EMBEDDED};
    Identification::ModuleId module;
    ErrorCode error;

    TechnicalError(Identification::ModuleId _module, ErrorCode _error)
        : module(_module)
        , error(_error){};

    uint32_t toUint() const
    {
        return (static_cast<uint8_t>(component) << 16) | (static_cast<uint8_t>(module) << 8)
               | static_cast<uint16_t>(error);
    }

    friend embedded_ostream& operator<<(embedded_ostream& os, const TechnicalError& error)
    {
        os << etl::showbase << etl::hex << error.toUint() << etl::dec << etl::noshowbase;
        return os;
    }
};
}; // namespace TechnicalErrorDefs

#endif /* NEMEIO_TECHNICALERROR_TECHNICALERRORDEFS_HPP_ */
