/*
 * strlen.c
 *
 *  Created on: 14 févr. 2022
 *      Author: thomas
 */

#include "strlen.h"

size_t strnlen(const char* s, size_t maxlen)
{
    size_t i;
    for(i = 0; i < maxlen; ++i)
    {
        if(s[i] == '\0')
        {
            break;
        }
    }
    return i;
}
