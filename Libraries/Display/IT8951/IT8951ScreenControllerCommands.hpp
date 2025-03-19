/*
 * IT8951ControllerCommands.hpp
 *
 *  Created on: Nov 23, 2017
 *      Author: Labo
 */

#ifndef __IT8951CONTROLLERCOMMANDS_HPP_
#define __IT8951CONTROLLERCOMMANDS_HPP_

// Preamble words
#define IT8951_PREAMBLE_WORD_COMMAND (0x6000)
#define IT8951_PREAMBLE_WORD_WRITE (0x0000)
#define IT8951_PREAMBLE_WORD_READ (0x1000)

//Built in Command Codes
#define IT8951_TCON_SYS_RUN_CMD (0x0001)
#define IT8951_TCON_STANDBY_CMD (0x0002)
#define IT8951_TCON_SLEEP_CMD (0x0003)
#define IT8951_TCON_REG_RD_CMD (0x0010)
#define IT8951_TCON_REG_WR_CMD (0x0011)
#define IT8951_TCON_MEM_BST_RD_T_CMD (0x0012)
#define IT8951_TCON_MEM_BST_RD_S_CMD (0x0013)
#define IT8951_TCON_MEM_BST_WR_CMD (0x0014)
#define IT8951_TCON_MEM_BST_END_CMD (0x0015)
#define IT8951_TCON_LD_IMG_CMD (0x0020)
#define IT8951_TCON_LD_IMG_AREA_CMD (0x0021)
#define IT8951_TCON_LD_IMG_END_CMD (0x0022)
#define IT8951_TCON_POWER_CMD (0x0038)
#define IT8951_TCON_FILL_RECT_CMD (0x003A)
#define IT8951_TCON_FSET_TEMP_CMD (0x0040)

#define IT8951_TCON_LOAD_IMG_MAX_DIM (2048)

//User defined command codes
#define USDEF_CMD_DPY_AREA_CMD (0x0034)
#define USDEF_CMD_DPY_AREA_CMD_FB_ADDR (0x0037)
#define USDEF_CMD_GET_DEV_INFO_CMD (0x0302)
#define USDEF_CMD_GET_JPGBUFF_CMD (0x003B)
#define USDEF_CMD_GET_JPGDEC_CMD (0x003C)

#define USDEF_CMD_GET_DEV_INFO_WORDS (20)
#define USDEF_CMD_GET_DEV_INFO_FW_STR_WORDS (8)
#define USDEF_CMD_GET_DEV_INFO_LUT_STR_WORDS (8)
#define USDEF_CMD_GET_JPGBUFF_WORDS (2)

#define DISPLAY_MODE_CLEAR (0)
#define DISPLAY_MODE_DU (1)
#define DISPLAY_MODE_GC16 (2)
#define DISPLAY_MODE_GL16 (3)
#define DISPLAY_MODE_A2 (4)
#define DISPLAY_MODE_DU4 (5)

//Rotate mode
#define IT8951_ROTATE_0 0
#define IT8951_ROTATE_90 1
#define IT8951_ROTATE_180 2
#define IT8951_ROTATE_270 3

//Pixel mode , BPP - Bit per Pixel
#define IT8951_2BPP 0
#define IT8951_3BPP 1
#define IT8951_4BPP 2
#define IT8951_8BPP 3

//Waveform Mode
#define IT8951_MODE_0 0
#define IT8951_MODE_1 1
#define IT8951_MODE_2 2
#define IT8951_MODE_3 3
#define IT8951_MODE_4 4
//Endian Type
#define IT8951_LDIMG_L_ENDIAN 0
#define IT8951_LDIMG_B_ENDIAN 1
//Auto LUT
#define IT8951_DIS_AUTO_LUT 0
#define IT8951_EN_AUTO_LUT 1
//LUT Engine Status
#define IT8951_ALL_LUTE_BUSY 0xFFFF

//-----------------------------------------------------------------------
// IT8951 TCon Registers defines
//-----------------------------------------------------------------------
//Register Base Address
#define DISPLAY_REG_BASE 0x1000 //Register RW access for I80 only
//Base Address of Basic LUT Registers
#define LUT0EWHRR_REG_ADDR (DISPLAY_REG_BASE + 0x00) //LUT0 Engine Width Height Reg
#define LUT0XYR_REG_ADDR (DISPLAY_REG_BASE + 0x40)   //LUT0 XY Reg
#define LUT0BADDR_REG_ADDR (DISPLAY_REG_BASE + 0x80) //LUT0 Base Address Reg
#define LUT0MFN_REG_ADDR (DISPLAY_REG_BASE + 0xC0)   //LUT0 Mode and Frame number Reg
#define LUT01AF_REG_ADDR (DISPLAY_REG_BASE + 0x114)  //LUT0 and LUT1 Active Flag Reg
//Update Parameter Setting Register
#define UP0SR_REG_ADDR (DISPLAY_REG_BASE + 0x134) //Update Parameter0 Setting Reg

#define UP1SR_REG_ADDR (DISPLAY_REG_BASE + 0x138)     //Update Parameter1 Setting Reg
#define UP1SR2_REG_ADDR (UP1SR_REG_ADDR + 2)          //Update Parameter1 Setting Reg 2
#define LUT0ABFRV_REG_ADDR (DISPLAY_REG_BASE + 0x13C) //LUT0 Alpha blend and Fill rectangle Value
#define UPBBADDR_REG_ADDR (DISPLAY_REG_BASE + 0x17C)  //Update Buffer Base Address
#define LUT0IMXY_REG_ADDR (DISPLAY_REG_BASE + 0x180)  //LUT0 Image buffer X/Y offset Reg
#define LUTAFSR_REG_ADDR (DISPLAY_REG_BASE + 0x224)   //LUT Status Reg (status of All LUT Engines)

#define BGVR_REG_ADDR (DISPLAY_REG_BASE + 0x250) //Bitmap (1bpp) image color table

#define UP1SR2_1BPP_IDX1 (1)    //1 bpp setting index
#define UP1SR2_1BPP_IDX2 (2)    //1 bpp setting index
#define UP1SR2_1BPP_ENABLED (1) //1 bpp setting enabled value

#define BGVR_BG_IDX (8) //Background color index
#define BGVR_FG_IDX (0) //Foreground color index

#define LUTAFSR_NOT_BUSY (0x0000) //LUT Status: zero = not busy

#define FSET_TEMP_CMD_ARG_GET (0x0000)
#define FSET_TEMP_CMD_ARG_SET (0x0001)

#define FILL_RECT_CMD_DMODE_SET_FB_IDX (8)
#define FILL_RECT_CMD_DMODE_SET_FB_EN (0)
#define FILL_RECT_CMD_DMODE_SET_FB_DIS (1)
#define FILL_RECT_CMD_DMODE_REFRESH_IDX (12)
#define FILL_RECT_CMD_DMODE_REFRESH_EN (1)
#define FILL_RECT_CMD_DMODE_REFRESH_DIS (0)

//-------System Registers----------------
#define SYS_REG_BASE 0x0000

//Address of System Registers
#define I80CPCR_REG_ADDR (SYS_REG_BASE + 0x04)
//-------Memory Converter Registers----------------
#define MCSR_BASE_ADDR 0x0200
#define MCSR_REG_ADDR (MCSR_BASE_ADDR + 0x0000)
#define LISAR_REG_ADDR (MCSR_BASE_ADDR + 0x0008)
#define LISAR2_REG_ADDR (LISAR_REG_ADDR + 2)

// Registers values
#define I80CPCR_ENABLE_PACKED (0x0001)

#define UP1SR (0x1138)


#endif /* __IT8951CONTROLLERCOMMANDS_HPP_ */
