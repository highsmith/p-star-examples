// Copyright Pololu Corporation.  For more information, see https://www.pololu.com/

/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license),
please contact mla_licensing@microchip.com
*******************************************************************************/

// This file contains the USB descriptors of the device.  The descriptors are
// read by the USB host when the device is plugged in.  They identify "Pololu
// Corporation" as the manufacturer, "P-Star" as the product name, and tell
// the USB host that this device has a single CDC ACM virtual serial port.
// They also contain the serial number from the P-Star's bootloader.
//
// If you make any major changes to these descriptors or if you run this code
// on a board that is not the P-Star, please be sure to change this file to
// use your own the vendor ID, product ID, and string descriptors.

#include "usb.h"
#include "usb_device_cdc.h"

// Device Descriptor
const USB_DEVICE_DESCRIPTOR device_dsc = {
    sizeof(USB_DEVICE_DESCRIPTOR),
    USB_DESCRIPTOR_DEVICE,
    0x0200,                 // USB Spec Release Number in BCD format
    CDC_DEVICE,             // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_config.h
    0x1FFB,                 // Vendor ID: Pololu Corporation
    0x2400,                 // Product ID: P-Star with one CDC ACM serial port
    0x0100,                 // Device release number in BCD format
    1,                      // Manufacturer string index
    2,                      // Product string index
    3,                      // Device serial number string index
    1                       // Number of possible configurations
};

const uint8_t configDescriptor1[67] = {
    // Configuration descriptor
    sizeof(USB_CONFIGURATION_DESCRIPTOR),
    USB_DESCRIPTOR_CONFIGURATION,
    sizeof(configDescriptor1), 0,
    2,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    0,                      // Configuration string index
    _DEFAULT | _SELF,       // Attributes, see usb_device.h
    50,                     // Max power consumption (2X mA)

    // CDC Interface Descriptor
    sizeof(USB_INTERFACE_DESCRIPTOR),
    USB_DESCRIPTOR_INTERFACE,
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    1,                      // Number of endpoints in this intf
    COMM_INTF,              // Class code
    ABSTRACT_CONTROL_MODEL, // Subclass code
    V25TER,                 // Protocol code
    0,                      // Interface string index

    // CDC Class-Specific Descriptors
    sizeof(USB_CDC_HEADER_FN_DSC),
    CS_INTERFACE,
    DSC_FN_HEADER,
    0x10, 0x01,

    sizeof(USB_CDC_ACM_FN_DSC),
    CS_INTERFACE,
    DSC_FN_ACM,
    USB_CDC_ACM_FN_DSC_VAL,

    sizeof(USB_CDC_UNION_FN_DSC),
    CS_INTERFACE,
    DSC_FN_UNION,
    CDC_COMM_INTF_ID,
    CDC_DATA_INTF_ID,

    sizeof(USB_CDC_CALL_MGT_FN_DSC),
    CS_INTERFACE,
    DSC_FN_CALL_MGT,
    0x00,
    CDC_DATA_INTF_ID,

    // CDC Notification Endpoint Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR),
    USB_DESCRIPTOR_ENDPOINT,
    CDC_COMM_EP | _EP_IN,       // EndpointAddress
    _INTERRUPT,                 // Attributes
    CDC_COMM_IN_EP_SIZE, 0x00,  // Endpoint size
    0x02,                       // Interval

    // CDC Data Interface Descriptor
    sizeof(USB_INTERFACE_DESCRIPTOR),
    USB_DESCRIPTOR_INTERFACE,
    CDC_DATA_INTF_ID,       // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this intf
    DATA_INTF,              // Class code
    0,                      // Subclass code
    NO_PROTOCOL,            // Protocol code
    0,                      // Interface string index

    // CDC Data OUT Endpoint Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR),
    USB_DESCRIPTOR_ENDPOINT,
    CDC_DATA_EP | _EP_OUT,      // EndpointAddress
    _BULK,                      // Attributes
    CDC_DATA_IN_EP_SIZE, 0x00,  // Packet size
    0x00,                       // Interval

    // CDC Data IN Endpoint Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR),
    USB_DESCRIPTOR_ENDPOINT,
    CDC_DATA_EP | _EP_IN,       // EndpointAddress
    _BULK,                      // Attributes
    0x40,0x00,                  // Packet size
    0x00,                       // Interval
};

// Language code string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[1];} sdLang = {
  sizeof(sdLang), USB_DESCRIPTOR_STRING, {0x0409}
};

// Manufacturer string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[18];} sdMfg = {
  sizeof(sdMfg), USB_DESCRIPTOR_STRING,
  {'P','o','l','o','l','u',' ','C','o','r','p','o','r','a','t','i','o','n'}
};

// Product string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[6];} sdProd = {
  sizeof(sdProd), USB_DESCRIPTOR_STRING,
  {'P','-','S','t','a','r'}
};

const uint8_t * const USB_CD_Ptr[] = {
    configDescriptor1
};

// Array of string descriptors
const uint8_t * const USB_SD_Ptr[USB_NUM_STRING_DESCRIPTORS] = {
    (const uint8_t *)&sdLang,
    (const uint8_t *)&sdMfg,
    (const uint8_t *)&sdProd,
    (const uint8_t *)0x80,   // Serial number from the P-Star bootloader
};
