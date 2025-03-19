/*
 * Image.hpp
 *
 *  Created on: 21 oct. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_IMAGES_IMAGE_HPP_
#define NEMEIO_IMAGES_IMAGE_HPP_

#include "IDataReader.hpp"

class Image
{
public:
    enum class eImageType
    {
        NONE,
        _1BPP,
        _2BPP,
        _3BPP,
        _4BPP,
        JPEG,
    };

    Image(eImageType type, IDataReader& content);
    virtual ~Image() = default;

    eImageType getType();
    IDataReader& getContent();

private:
    eImageType mType;
    IDataReader& mContent;
};

#endif /* NEMEIO_IMAGES_IMAGE_HPP_ */
