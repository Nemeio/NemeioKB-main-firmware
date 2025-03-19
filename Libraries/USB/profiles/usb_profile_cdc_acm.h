/*
 * usb_profile_cdc_acm.h
 *
 *  Created on: Jan 28, 2019
 *      Author: Labo
 */

#ifndef USB_PROFILES_USB_PROFILE_CDC_ACM_H_
#define USB_PROFILES_USB_PROFILE_CDC_ACM_H_

#include "usb_composite_api.h"
#include "usbd_cdc.h"

#define USB_CDC_ACM_PROFILE_INTERFACE_NUMBER 2
#define USB_CDC_ACM_PROFILE_SIZE \
    (USB_INTERFACE_DESCRIPTOR_SIZE * 2 + USB_ENDPOINT_DESCRIPTOR_SIZE * 3 \
     + USB_CS_INTERFACE_5_DESCRIPTOR_SIZE * 3 + USB_CS_INTERFACE_4_DESCRIPTOR_SIZE)

/* Interface descriptor : Communication class*/
#define CDC_ACM_INTERFACE_0_ALTERNATE_SETTINGS 0x00 /* bAlternateSetting: Alternate setting */
#define CDC_ACM_INTERFACE_0_ENDPOINTS_NUMBER 0x01   /* bNumEndpoints: One endpoint used */
#define CDC_ACM_INTERFACE_0_INTERFACE_CLASS \
    0x02 /* bInterfaceClass: Communication Interface Class */
#define CDC_ACM_INTERFACE_0_INTERFACE_SUBCLASS 0x02 /* bInterfaceSubClass: Abstract Control Model */
#define CDC_ACM_INTERFACE_0_INTERFACE_PROTOCOL 0x01 /* bInterfaceProtocol: Common AT commands */
#define CDC_ACM_INTERFACE_0_INTERFACE 0x00          /* iInterface: */

#define CDC_ACM_INTERFACE_0_CS_HEADER_SUBTYPE 0x00 /* bDescriptorSubtype: Header Func Desc */
#define CDC_ACM_INTERFACE_0_CS_HEADER_PARAM1 0x10  /* bcdCDC: spec release number */
#define CDC_ACM_INTERFACE_0_CS_HEADER_PARAM2 0x01  /* bcdCDC: spec release number */

#define CDC_ACM_INTERFACE_0_CS_CALL_MGT_SUBTYPE \
    0x01 /* bDescriptorSubtype: Call Management Func Desc */
#define CDC_ACM_INTERFACE_0_CS_CALL_MGT_PARAM1 0x00 /* bmCapabilities: D0+D1 */
#define CDC_ACM_INTERFACE_0_CS_CALL_MGT_PARAM2 0x01 /* bDataInterface: 1 */

#define CDC_ACM_INTERFACE_0_CS_ACM_SUBTYPE \
    0x02 /* bDescriptorSubtype: Abstract Control Management desc */
#define CDC_ACM_INTERFACE_0_CS_ACM_PARAM1 0x02 /* bmCapabilities */

#define CDC_ACM_INTERFACE_0_CS_UNION_SUBTYPE 0x06 /* bDescriptorSubtype: Union func desc */
#define CDC_ACM_INTERFACE_0_CS_UNION_PARAM1 \
    0x00 /* bMasterInterface: Communication class interface */
#define CDC_ACM_INTERFACE_0_CS_UNION_PARAM2 0x01 /* bSlaveInterface0: Data Class Interface */

/* Endpoint descriptor : Notification */
#define CDC_ACM_ENDPOINT_0_ADDRESS CDC_CMD_EP                          /* bEndpointAddress */
#define CDC_ACM_ENDPOINT_0_ATTRIBUTES USB_ENDPOINT_ATTRIBUTE_INTERRUPT /* bmAttributes: Interrupt */
#define CDC_ACM_ENDPOINT_0_W_MAX_PACKET_SIZE CDC_CMD_PACKET_SIZE       /* wMaxPacketSize: */
#define CDC_ACM_ENDPOINT_0_INTERVAL 0x10                               /* bInterval: */

/* Interface descriptor : Data class*/
#define CDC_ACM_INTERFACE_1_ALTERNATE_SETTINGS 0x00 /* bAlternateSetting: Alternate setting */
#define CDC_ACM_INTERFACE_1_ENDPOINTS_NUMBER 0x02   /* bNumEndpoints: Two endpoints used */
#define CDC_ACM_INTERFACE_1_INTERFACE_CLASS 0x0A    /* bInterfaceClass: CDC */
#define CDC_ACM_INTERFACE_1_INTERFACE_SUBCLASS 0x00 /* bInterfaceSubClass: */
#define CDC_ACM_INTERFACE_1_INTERFACE_PROTOCOL 0x00 /* bInterfaceProtocol: */
#define CDC_ACM_INTERFACE_1_INTERFACE 0x00          /* iInterface: */

/* Endpoint descriptor : Data OUT */
#define CDC_ACM_ENDPOINT_1_ADDRESS CDC_OUT_EP                            /* bEndpointAddress */
#define CDC_ACM_ENDPOINT_1_ATTRIBUTES USB_ENDPOINT_ATTRIBUTE_BULK        /* bmAttributes: Bulk */
#define CDC_ACM_ENDPOINT_1_W_MAX_PACKET_SIZE CDC_DATA_FS_MAX_PACKET_SIZE /* wMaxPacketSize: */
#define CDC_ACM_ENDPOINT_1_INTERVAL 0x00 /* bInterval: ignore for Bulk transfer */

/* Endpoint descriptor : Data IN */
#define CDC_ACM_ENDPOINT_2_ADDRESS CDC_IN_EP                             /* bEndpointAddress */
#define CDC_ACM_ENDPOINT_2_ATTRIBUTES USB_ENDPOINT_ATTRIBUTE_BULK        /* bmAttributes: Bulk */
#define CDC_ACM_ENDPOINT_2_W_MAX_PACKET_SIZE CDC_DATA_FS_MAX_PACKET_SIZE /* wMaxPacketSize: */
#define CDC_ACM_ENDPOINT_2_INTERVAL 0x00 /* bInterval: ignore for Bulk transfer */

#define USB_CDC_ACM_PROFILE(interface_index) \
    USB_INTERFACE_DESCRIPTOR(interface_index, \
                             CDC_ACM_INTERFACE_0_ALTERNATE_SETTINGS, \
                             CDC_ACM_INTERFACE_0_ENDPOINTS_NUMBER, \
                             CDC_ACM_INTERFACE_0_INTERFACE_CLASS, \
                             CDC_ACM_INTERFACE_0_INTERFACE_SUBCLASS, \
                             CDC_ACM_INTERFACE_0_INTERFACE_PROTOCOL, \
                             CDC_ACM_INTERFACE_0_INTERFACE), \
        USB_CS_INTERFACE_5_DESCRIPTOR(CDC_ACM_INTERFACE_0_CS_HEADER_SUBTYPE, \
                                      CDC_ACM_INTERFACE_0_CS_HEADER_PARAM1, \
                                      CDC_ACM_INTERFACE_0_CS_HEADER_PARAM2), \
        USB_CS_INTERFACE_5_DESCRIPTOR(CDC_ACM_INTERFACE_0_CS_CALL_MGT_SUBTYPE, \
                                      CDC_ACM_INTERFACE_0_CS_CALL_MGT_PARAM1, \
                                      CDC_ACM_INTERFACE_0_CS_CALL_MGT_PARAM2), \
        USB_CS_INTERFACE_4_DESCRIPTOR(CDC_ACM_INTERFACE_0_CS_ACM_SUBTYPE, \
                                      CDC_ACM_INTERFACE_0_CS_ACM_PARAM1), \
        USB_CS_INTERFACE_5_DESCRIPTOR(CDC_ACM_INTERFACE_0_CS_UNION_SUBTYPE, \
                                      CDC_ACM_INTERFACE_0_CS_UNION_PARAM1, \
                                      CDC_ACM_INTERFACE_0_CS_UNION_PARAM2), \
        USB_ENDPOINT_DESCRIPTOR(CDC_ACM_ENDPOINT_0_ADDRESS, \
                                CDC_ACM_ENDPOINT_0_ATTRIBUTES, \
                                CDC_ACM_ENDPOINT_0_W_MAX_PACKET_SIZE, \
                                CDC_ACM_ENDPOINT_0_INTERVAL), \
        USB_INTERFACE_DESCRIPTOR((interface_index + 1), \
                                 CDC_ACM_INTERFACE_1_ALTERNATE_SETTINGS, \
                                 CDC_ACM_INTERFACE_1_ENDPOINTS_NUMBER, \
                                 CDC_ACM_INTERFACE_1_INTERFACE_CLASS, \
                                 CDC_ACM_INTERFACE_1_INTERFACE_SUBCLASS, \
                                 CDC_ACM_INTERFACE_1_INTERFACE_PROTOCOL, \
                                 CDC_ACM_INTERFACE_1_INTERFACE), \
        USB_ENDPOINT_DESCRIPTOR(CDC_ACM_ENDPOINT_1_ADDRESS, \
                                CDC_ACM_ENDPOINT_1_ATTRIBUTES, \
                                CDC_ACM_ENDPOINT_1_W_MAX_PACKET_SIZE, \
                                CDC_ACM_ENDPOINT_1_INTERVAL), \
        USB_ENDPOINT_DESCRIPTOR(CDC_ACM_ENDPOINT_2_ADDRESS, \
                                CDC_ACM_ENDPOINT_2_ATTRIBUTES, \
                                CDC_ACM_ENDPOINT_2_W_MAX_PACKET_SIZE, \
                                CDC_ACM_ENDPOINT_2_INTERVAL)


#endif /* USB_PROFILES_USB_PROFILE_CDC_ACM_H_ */
