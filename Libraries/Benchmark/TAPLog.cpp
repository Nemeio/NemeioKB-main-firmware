/*
 * TAPLog.cpp
 *
 *  Created on: 10 mai 2021
 *      Author: thomas
 */

#include "TAPLog.hpp"
#include "assertTools.h"

TAPLog::TAPLog(AbstractDataTransport& transport)
    : mTransport(transport)
{
}

void TAPLog::newTestSuite(etl::string_view name, size_t nbTest)
{
    mOsStream.clear();
    mOsStream << name << embedded_io::endl;
    mOsStream << "1..." << nbTest << embedded_io::endl;
    print(mOsStream.str());
    mNbTests = nbTest;
    mCurrentTest = 0;
}

void TAPLog::onSuccess(etl::string_view name, etl::string_view description)
{
    increaseTestNumber();
    mOsStream << "ok " << mCurrentTest << " " << name << embedded_io::endl;
    if(description.size())
    {
        mOsStream << '\t' << description << embedded_io::endl;
    }

    print(mOsStream.str());
}

void TAPLog::onFailure(etl::string_view name, etl::string_view description)
{
    increaseTestNumber();
    mOsStream << "not ok " << mCurrentTest << " " << name << embedded_io::endl
              << '\t' << description << embedded_io::endl;
    print(mOsStream.str());
}

void TAPLog::print(etl::string_view str)
{
    mTransport.sendData(reinterpret_cast<const uint8_t*>(str.data()), str.size());
}

void TAPLog::increaseTestNumber()
{
    ++mCurrentTest;
    mOsStream.clear();
    ASSERT(mCurrentTest <= mNbTests);
}
