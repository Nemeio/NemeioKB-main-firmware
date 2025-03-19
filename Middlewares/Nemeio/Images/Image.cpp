/*
 * Image.cpp
 *
 *  Created on: 21 oct. 2021
 *      Author: thomas
 */

#include "Image.hpp"

Image::Image(eImageType type, IDataReader& content)
    : mType(type)
    , mContent(content)
{
}

Image::eImageType Image::getType()
{
    return mType;
}

IDataReader& Image::getContent()
{
    return mContent;
}
