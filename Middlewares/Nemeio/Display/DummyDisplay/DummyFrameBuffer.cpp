/*
 * DummyFrameBuffer.cpp
 *
 *  Created on: 3 nov. 2021
 *      Author: thomas
 */

#include "DummyFrameBuffer.hpp"

bool DummyFrameBuffer::clearContent()
{
    return true;
}

bool DummyFrameBuffer::loadImage(Image& image)
{
    return true;
}

bool DummyFrameBuffer::loadImage(ScreenDefs::Window window, Image& image)
{
    return true;
}

uint32_t DummyFrameBuffer::getStartAddress()
{
    return 0;
}

Image::eImageType DummyFrameBuffer::getContentType()
{
    return Image::eImageType::NONE;
}
