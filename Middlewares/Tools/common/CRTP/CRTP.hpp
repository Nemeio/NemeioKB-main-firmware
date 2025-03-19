/*
 * CRTP.hpp
 *
 *  Created on: Jan 27, 2020
 *      Author: Labo
 */

#ifndef TOOLS_CRTP_CRTP_HPP_
#define TOOLS_CRTP_CRTP_HPP_

template<typename T>
class CRTP
{
public:
    CRTP() = default;
    virtual ~CRTP() = default;

protected:
    T& underlying();

    const T& underlying() const;
};

template<typename T>
T& CRTP<T>::underlying()
{
    return static_cast<T&>(*this);
}

template<typename T>
const T& CRTP<T>::underlying() const
{
    return static_cast<const T&>(*this);
}

#endif /* TOOLS_CRTP_CRTP_HPP_ */
