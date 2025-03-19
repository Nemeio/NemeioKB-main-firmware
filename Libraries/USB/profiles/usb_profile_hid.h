/*
 * usb_hid.h
 *
 *  Created on: 3 mai 2018
 *      Author: thomas
 */

#ifndef USB_USB_HID_H_
#define USB_USB_HID_H_

#include "usb_composite_api.h"


/* Interface descriptor : Classic HID */
#define HID_ALTERNATE_SETTINGS 0x00
#define HID_ENDPOINTS_NUMBER 0x01
#define HID_INTERFACE_CLASS 0x03
#define HID_INTERFACE_SUBCLASS 0x00
#define HID_INTERFACE_PROTOCOL 0x01
#define HID_INTERFACE 0x00

/* Endpoint descriptor */
#define HID_ENDPOINT_0_ADDRESS HID_EPIN_ADDR
#define HID_ENDPOINT_0_ATTRIBUTES USB_ENDPOINT_ATTRIBUTE_INTERRUPT
#define HID_ENDPOINT_0_W_MAX_PACKET_SIZE HID_EPIN_SIZE
#define HID_ENDPOINT_0_INTERVAL HID_FS_BINTERVAL


#define USB_HID_PROFILE(interface_index) \
    USB_INTERFACE_DESCRIPTOR(interface_index, \
                             HID_ALTERNATE_SETTINGS, \
                             HID_ENDPOINTS_NUMBER, \
                             HID_INTERFACE_CLASS, \
                             HID_INTERFACE_SUBCLASS, \
                             HID_INTERFACE_PROTOCOL, \
                             HID_INTERFACE), \
        USB_HID_DESCRIPTOR(), \
        USB_ENDPOINT_DESCRIPTOR(HID_ENDPOINT_0_ADDRESS, \
                                HID_ENDPOINT_0_ATTRIBUTES, \
                                HID_ENDPOINT_0_W_MAX_PACKET_SIZE, \
                                HID_ENDPOINT_0_INTERVAL)

#define USB_HID_PROFILE_SIZE \
    (USB_INTERFACE_DESCRIPTOR_SIZE + USB_HID_DESCRIPTOR_SIZE + USB_ENDPOINT_DESCRIPTOR_SIZE)
#define USB_HID_PROFILE_INTERFACE_NUMBER 0x01

#endif /* USB_USB_HID_H_ */
