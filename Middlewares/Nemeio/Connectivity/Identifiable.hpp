/*
 * Identifiable.hpp
 *
 *  Created on: Jun 25, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_CONNECTIVITY_IDENTIFIABLE_HPP_
#define NEMEIO_CONNECTIVITY_IDENTIFIABLE_HPP_

class Identifiable
{
public:
    Identifiable() = default;
    virtual ~Identifiable() = default;

    bool operator==(const Identifiable& dataSource) const;
};

inline bool Identifiable::operator==(const Identifiable& dataSource) const
{
    // For now we consider the AbstractDataSources are identical only if they are the same instance.
    // The operator == is overloaded to avoid comparing the pointers outside of the class and will allow
    // to change the comparison method eventually.
    return this == &dataSource;
}

#endif /* NEMEIO_CONNECTIVITY_IDENTIFIABLE_HPP_ */
