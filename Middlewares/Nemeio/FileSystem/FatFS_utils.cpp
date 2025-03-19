/*
 * FatFS_utils.cpp
 *
 *  Created on: Jul 24, 2018
 *      Author: Labo
 */

#include "FatFS_utils.hpp"

/* Copy of pattern matching utils from FatFS */

/*----------------------------------------------------------------------------/
/  FatFs - Generic FAT Filesystem Module  R0.13                               /
/-----------------------------------------------------------------------------/
/
/ Copyright (C) 2017, ChaN, all right reserved.
/
/ FatFs module is an open source software. Redistribution and use of FatFs in
/ source and binary forms, with or without modification, are permitted provided
/ that the following condition is met:
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright owner or contributors be NOT LIABLE for any damages caused
/ by use of this software.
/
/----------------------------------------------------------------------------*/
typedef unsigned char BYTE;
typedef unsigned short WCHAR;
/*-----------------------------------------------------------------------*/
/* Pattern matching                                                      */
/*-----------------------------------------------------------------------*/
#define FF_CODE_PAGE 932

/* Macros for table definitions */
#define TBL_DC932 \
    { \
        0x81, 0x9F, 0xE0, 0xFC, 0x40, 0x7E, 0x80, 0xFC, 0x00, 0x00 \
    }
#define MERGE_2STR(a, b) a##b
#define MKCVTBL(hd, cp) MERGE_2STR(hd, cp)
static const BYTE DbcTbl[] = MKCVTBL(TBL_DC, FF_CODE_PAGE);

/* ASCII code support macros */
#define IsUpper(c) ((c) >= 'A' && (c) <= 'Z')
#define IsLower(c) ((c) >= 'a' && (c) <= 'z')
#define IsDigit(c) ((c) >= '0' && (c) <= '9')

/* Test if the character is DBC 1st byte */
static int dbc_1st(BYTE c)
{
#if FF_CODE_PAGE == 0 /* Variable code page */
    if(DbcTbl && c >= DbcTbl[0])
    {
        if(c <= DbcTbl[1])
            return 1; /* 1st byte range 1 */
        if(c >= DbcTbl[2] && c <= DbcTbl[3])
            return 1; /* 1st byte range 2 */
    }
#elif FF_CODE_PAGE >= 900 /* DBCS fixed code page */
    if(c >= DbcTbl[0])
    {
        if(c <= DbcTbl[1])
            return 1;
        if(c >= DbcTbl[2] && c <= DbcTbl[3])
            return 1;
    }
#else                     /* SBCS fixed code page */
    if(c)
        return 0; /* Always false */
#endif
    return 0;
}

/* Test if the character is DBC 2nd byte */
static int dbc_2nd(BYTE c)
{
#if FF_CODE_PAGE == 0 /* Variable code page */
    if(DbcTbl && c >= DbcTbl[4])
    {
        if(c <= DbcTbl[5])
            return 1; /* 2nd byte range 1 */
        if(c >= DbcTbl[6] && c <= DbcTbl[7])
            return 1; /* 2nd byte range 2 */
        if(c >= DbcTbl[8] && c <= DbcTbl[9])
            return 1; /* 2nd byte range 3 */
    }
#elif FF_CODE_PAGE >= 900 /* DBCD fixed code page */
    if(c >= DbcTbl[4])
    {
        if(c <= DbcTbl[5])
            return 1;
        if(c >= DbcTbl[6] && c <= DbcTbl[7])
            return 1;
        if(c >= DbcTbl[8] && c <= DbcTbl[9])
            return 1;
    }
#else                     /* SBCS fixed code page */
    if(c)
        return 0; /* Always false */
#endif
    return 0;
}

static WCHAR get_achar(                  /* Get a character and advances ptr 1 or 2 */
                       const TCHAR** ptr /* Pointer to pointer to the SBCS/DBCS/Unicode string */
)
{
    WCHAR chr;

#if FF_LFN_UNICODE && FF_USE_LFN  /* Unicode API */
    chr = ff_wtoupper(*(*ptr)++); /* Get a Unicode char and to upper */
#else                             /* ANSI/OEM API */
    chr = (BYTE) * (*ptr)++; /* Get a byte */
    if(IsLower(chr))
        chr -= 0x20; /* To upper ASCII char */
#if FF_CODE_PAGE == 0
    if(ExCvt && chr >= 0x80)
        chr = ExCvt[chr - 0x80]; /* To upper SBCS extended char */
#elif FF_CODE_PAGE < 900
    if(chr >= 0x80)
        chr = ExCvt[chr - 0x80]; /* To upper SBCS extended char */
#endif
#if FF_CODE_PAGE == 0 || FF_CODE_PAGE >= 900
    if(dbc_1st((BYTE) chr) && dbc_2nd((BYTE) * *ptr))
    { /* Get DBC 2nd byte if needed */
        chr = chr << 8 | (BYTE) * (*ptr)++;
    }
#endif
#endif
    return chr;
}


int pattern_matching(                  /* 0:not matched, 1:matched */
                     const TCHAR* pat, /* Matching pattern */
                     const TCHAR* nam, /* String to be tested */
                     int skip,         /* Number of pre-skip chars (number of ?s) */
                     int inf           /* Infinite search (* specified) */
)
{
    const TCHAR *pp, *np;
    WCHAR pc, nc;
    int nm, nx;


    while(skip--)
    { /* Pre-skip name chars */
        if(!get_achar(&nam))
            return 0; /* Branch mismatched if less name chars */
    }
    if(!*pat && inf)
        return 1; /* (short circuit) */

    do
    {
        pp = pat;
        np = nam; /* Top of pattern and name to match */
        for(;;)
        {
            if(*pp == '?' || *pp == '*')
            { /* Wildcard? */
                nm = nx = 0;
                do
                { /* Analyze the wildcard chars */
                    if(*pp++ == '?')
                        nm++;
                    else
                        nx = 1;
                } while(*pp == '?' || *pp == '*');
                if(pattern_matching(pp, np, nm, nx))
                    return 1; /* Test new branch (recurs upto number of wildcard blocks in the pattern) */
                nc = *np;
                break; /* Branch mismatched */
            }
            pc = get_achar(&pp); /* Get a pattern char */
            nc = get_achar(&np); /* Get a name char */
            if(pc != nc)
                break; /* Branch mismatched? */
            if(pc == 0)
                return 1; /* Branch matched? (matched at end of both strings) */
        }
        get_achar(&nam); /* nam++ */
    } while(inf && nc);  /* Retry until end of name if infinite search is specified */

    return 0;
}
