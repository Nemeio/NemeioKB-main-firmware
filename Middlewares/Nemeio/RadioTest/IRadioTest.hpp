/*
 * IRadioTest.hpp
 *
 *  Created on: 1 déc. 2021
 *      Author: eskoric
 */

#ifndef NEMEIO_RADIOTEST_IRADIOTEST_HPP_
#define NEMEIO_RADIOTEST_IRADIOTEST_HPP_

#include "BLEDefs.hpp"

class IRadioTest
{
public:
    IRadioTest() = default;
    virtual ~IRadioTest() = default;

    virtual bool runMode(BLEDefs::TestMode mode) = 0;
};

#endif /* NEMEIO_RADIOTEST_IRADIOTEST_HPP_ */
