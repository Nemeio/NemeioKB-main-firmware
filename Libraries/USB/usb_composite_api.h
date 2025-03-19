/*
 * usb_interface.h
 *
 *  Created on: 3 mai 2018
 *      Author: thomas
 */

#ifndef USB_USB_COMPOSITE_API_H_
#define USB_USB_COMPOSITE_API_H_

#define USB_DESC_TYPE_CS_INTERFACE 0x24


#define USB_ENDPOINT_ATTRIBUTE_CONTROL 0x00
#define USB_ENDPOINT_ATTRIBUTE_ISOCHRONOUS 0x01
#define USB_ENDPOINT_ATTRIBUTE_BULK 0x02
#define USB_ENDPOINT_ATTRIBUTE_INTERRUPT 0x03

#define USB_HID_DESCRIPTOR() \
    USB_HID_DESC_SIZ, HID_DESCRIPTOR_TYPE, 0x11, 0x01, 0x00, 0x01, HID_REPORT_DESC, \
        HID_KB_REPORT_DESC_SIZE, 0x00

#define USB_HID_DESCRIPTOR_SIZE USB_HID_DESC_SIZ

#define USB_INTERFACE_DESCRIPTOR(number, \
                                 alternate_settings, \
                                 nb_endpoints, \
                                 class, \
                                 subclass, \
                                 protocol, \
                                 interface) \
    USB_LEN_IF_DESC, USB_DESC_TYPE_INTERFACE, number, alternate_settings, nb_endpoints, class, \
        subclass, protocol, interface

#define USB_INTERFACE_DESCRIPTOR_SIZE USB_LEN_IF_DESC


#define USB_ENDPOINT_DESCRIPTOR(endpoint_address, attributes, max_packet_size, interval) \
    USB_LEN_EP_DESC, USB_DESC_TYPE_ENDPOINT, endpoint_address, attributes, \
        (max_packet_size & 0xFF), (max_packet_size >> 8), interval

#define USB_ENDPOINT_DESCRIPTOR_SIZE USB_LEN_EP_DESC

#define USB_CS_INTERFACE_5_DESCRIPTOR(subtype, param1, param2) \
    0x05, USB_DESC_TYPE_CS_INTERFACE, subtype, param1, param2

#define USB_CS_INTERFACE_5_DESCRIPTOR_SIZE 5

#define USB_CS_INTERFACE_4_DESCRIPTOR(subtype, param1) \
    0x04, USB_DESC_TYPE_CS_INTERFACE, subtype, param1

#define USB_CS_INTERFACE_4_DESCRIPTOR_SIZE 4


#define USB_INTERFACE_ASSOCIATION_DESCRIPTOR(first_interface, \
                                             interface_number, \
                                             _class, \
                                             subclass, \
                                             protocol, \
                                             str_idx) \
    0x08, 0x0B, first_interface, interface_number, _class, subclass, protocol, str_idx

#define USB_INTERFACE_ASSOCIATION_DESCRIPTOR_SIZE 0x08

#define USB_COMPOSITE_CONFIGURATION(size, interface_number) \
    0x09, USB_DESC_TYPE_CONFIGURATION, (0x09 + size), 0x00, interface_number, \
        0x01, /*bConfigurationValue: Configuration value*/ \
        0x00, /*iConfiguration: Index of string descriptor describing the configuration*/ \
        0xE0, /*bmAttributes: bus powered */ \
        0xFA  /*MaxPower 100 mA: this current is used for detecting Vbus*/

#endif /* USB_USB_COMPOSITE_API_H_ */
