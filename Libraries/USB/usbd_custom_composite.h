/*
 * usbd_custom_rndis_hid_desc.h
 *
 *  Created on: 21 mars 2017
 *      Author: cgarnier
 */

#ifndef USBD_CUSTOM_RNDIS_HID_DESC_H_
#define USBD_CUSTOM_RNDIS_HID_DESC_H_

/*Includes ************************************************/
#include <fs.hpp>
#include "usbd_def.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

extern USBD_ClassTypeDef USBD_CUSTOM_ACM_HID;

uint8_t USBD_CUSTOM_ACM_HID_CDC_Send(USBD_HandleTypeDef* pdev, uint8_t* pData, size_t len);
uint8_t USBD_CUSTOM_ACM_HID_HID_SendReport(USBD_HandleTypeDef* pdev,
                                           const uint8_t* report,
                                           size_t len);

void USBD_CUSTOM_ACM_HID_HID_Init();

#endif /* USBD_CUSTOM_RNDIS_HID_DESC_H_ */
