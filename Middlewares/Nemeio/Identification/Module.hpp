/*
 * Component.hpp
 *
 *  Created on: Feb 25, 2020
 *      Author: Labo
 */

#ifndef NEMEIO_IDENTIFICATION_MODULE_HPP_
#define NEMEIO_IDENTIFICATION_MODULE_HPP_

#include "Identification.hpp"

template<Identification::ModuleId ID>
class Module
{
public:
    Module() = default;
    virtual ~Module() = default;

    Identification::ModuleId getModuleId() const;
};

template<Identification::ModuleId ID>
inline Identification::ModuleId Module<ID>::getModuleId() const
{
    return ID;
}

#endif /* NEMEIO_IDENTIFICATION_MODULE_HPP_ */
