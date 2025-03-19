/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "S25FL128L.hpp"
#include "S25FL128L_defs.hpp"
#include "FatFS_custom.h" /* FatFs lower layer API */

#include "string.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM 0 /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC 1 /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB 2 /* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    if(0 == pdrv)
    {
        return 0;
    }
    return STA_NOINIT;
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    int iReturn = STA_NOINIT;
    if(0 == pdrv)
    {
        if(!S25FL128L::GetInstance().init_ff_S25FL128L())
            iReturn = STA_OK;
    }
    return iReturn;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv,    /* Physical drive number to identify the drive */
                  BYTE* buff,   /* Data buffer to store read data */
                  DWORD sector, /* Start sector in LBA */
                  UINT count    /* Number of sectors to read */
)
{
    if(0 == pdrv)
    {
        //		printf("About to read %d sector(s) (%d bytes) to a buffer of (4096)\n", count, count * SECTOR_SIZE_IN_BYTES);

        while(count > 0)
        {
            S25FL128L::GetInstance().read_S25FL128L(sector * SECTOR_SIZE_IN_BYTES,
                                                    buff,
                                                    SECTOR_SIZE_IN_BYTES);
            --count;
            ++sector;
            buff += SECTOR_SIZE_IN_BYTES;
        }
        return RES_OK;
    }
    return RES_PARERR;
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(BYTE pdrv,        /* Physical drive number to identify the drive */
                   const BYTE* buff, /* Data to be written */
                   DWORD sector,     /* Start sector in LBA */
                   UINT count        /* Number of sectors to write */
)
{
    //	printf("About to WRITE : %d sector(s) (%d bytes) from a buffer of (4096)\n", count, count * SECTOR_SIZE_IN_BYTES);

    if(0 == pdrv)
    {
        while(count > 0)
        {
            S25FL128L::GetInstance().eraseSector_S25FL128L(sector * SECTOR_SIZE_IN_BYTES,
                                                           SECTOR_SIZE_IN_BYTES);
            S25FL128L::GetInstance().write_S25FL128L(sector * SECTOR_SIZE_IN_BYTES,
                                                     SECTOR_SIZE_IN_BYTES,
                                                     (unsigned char*) buff);
            ++sector;
            --count;
            buff += SECTOR_SIZE_IN_BYTES;
        }

        return RES_OK;
    }
    return RES_PARERR;
}


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
                   BYTE cmd,  /* Control code */
                   void* buff /* Buffer to send/receive control data */
)
{
    DRESULT res = RES_PARERR;

    switch(cmd)
    {
    case CTRL_SYNC:
        res = RES_OK;
        return res;

    case GET_SECTOR_COUNT:
        *(DWORD*) buff = FLASH_NUMBER_OF_SECTORS;
        res = RES_OK;
        return res;

    case GET_SECTOR_SIZE:
        *(WORD*) buff = SECTOR_SIZE_IN_BYTES;
        res = RES_OK;
        return res;

    case GET_BLOCK_SIZE:
        *(DWORD*) buff = 1; //BLOCK_SIZE_IN_BYTES / SECTOR_SIZE_IN_BYTES;
        res = RES_OK;
        return res;
    }

    return res;
}
