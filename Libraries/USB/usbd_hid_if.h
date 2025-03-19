/*
 * usbd_hid_if.h
 *
 *  Created on: Sep 17, 2019
 *      Author: Labo
 */

#ifndef USB_USBD_HID_IF_H_
#define USB_USBD_HID_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_hid.h"

extern USBD_HID_ItfTypeDef USBD_HID_Interface;

void HID_InitInterface(void* pListener,
                       void (*keyLockStatesChangedCallback)(void*, const uint8_t*, size_t));

#ifdef __cplusplus
}
#endif

#endif /* USB_USBD_HID_IF_H_ */
