/*
 * assert.hpp
 *
 *  Created on: Jan 24, 2019
 *      Author: Labo
 */

#ifndef ASSERTTOOLS_HPP_
#define ASSERTTOOLS_HPP_

#include <cassert>

#define ASSERT(expr) \
    { \
        if(expr) \
        { \
        } \
        else \
        { \
            assert(false); \
        } \
    }

#endif /* ASSERTTOOLS_HPP_ */
