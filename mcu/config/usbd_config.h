/* Auto-generated config file usbd_config.h */
#ifndef USBD_CONFIG_H
#define USBD_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// ---- USB Device Stack Core Options ----

// <q> High Speed Support
// <i> Enable high speed specific descriptors support, e.g., DeviceQualifierDescriptor and OtherSpeedConfiguration Descriptor.
// <i> High speed support require descriptors description array on start, for LS/FS and HS support in first and second place.
// <id> usbd_hs_sp
#ifndef CONF_USBD_HS_SP
#define CONF_USBD_HS_SP 0
#endif

// ---- USB Device Stack Vendor Options ----

// <e> Enable String Descriptors
// <id> usb_vendor_str_en
#ifndef CONF_USB_VENDOR_STR_EN
#define CONF_USB_VENDOR_STR_EN 0
#endif
// <s> Language IDs
// <i> Language IDs in c format, split by comma (E.g., 0x0409 ...)
// <id> usb_vendor_langid
#ifndef CONF_USB_VENDOR_LANGID
#define CONF_USB_VENDOR_LANGID "0x0409"
#endif

#ifndef CONF_USB_VENDOR_LANGID_DESC
#define CONF_USB_VENDOR_LANGID_DESC
#endif
// </e>

// <h> Vendor Class Device Descriptor

// <o> bcdUSB
// <0x0200=> USB 2.0 version
// <0x0210=> USB 2.1 version
// <id> usb_vendor_bcdusb
#ifndef CONF_USB_VENDOR_BCDUSB
#define CONF_USB_VENDOR_BCDUSB 0x200
#endif

// <o> bMaxPackeSize0
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_vendor_bmaxpksz0
#ifndef CONF_USB_VENDOR_BMAXPKSZ0
#define CONF_USB_VENDOR_BMAXPKSZ0 0x40
#endif

// <o> idVender <0x0000-0xFFFF>
// <id> usb_vendor_idvender
#ifndef CONF_USB_VENDOR_IDVENDER
#define CONF_USB_VENDOR_IDVENDER 0x3eb
#endif

// <o> idProduct <0x0000-0xFFFF>
// <id> usb_vendor_idproduct
#ifndef CONF_USB_VENDOR_IDPRODUCT
#define CONF_USB_VENDOR_IDPRODUCT 0x2423
#endif

// <o> bcdDevice <0x0000-0xFFFF>
// <id> usb_vendor_bcddevice
#ifndef CONF_USB_VENDOR_BCDDEVICE
#define CONF_USB_VENDOR_BCDDEVICE 0x100
#endif

// <e> Enable string descriptor of iManufact
// <id> usb_vendor_imanufact_en
#ifndef CONF_USB_VENDOR_IMANUFACT_EN
#define CONF_USB_VENDOR_IMANUFACT_EN 0
#endif

#ifndef CONF_USB_VENDOR_IMANUFACT
#define CONF_USB_VENDOR_IMANUFACT (CONF_USB_VENDOR_IMANUFACT_EN * (CONF_USB_VENDOR_IMANUFACT_EN))
#endif

// <s> Unicode string of iManufact
// <id> usb_vendor_imanufact_str
#ifndef CONF_USB_VENDOR_IMANUFACT_STR
#define CONF_USB_VENDOR_IMANUFACT_STR "Atmel"
#endif

#ifndef CONF_USB_VENDOR_IMANUFACT_STR_DESC
#define CONF_USB_VENDOR_IMANUFACT_STR_DESC
#endif

// </e>

// <e> Enable string descriptor of iProduct
// <id> usb_vendor_iproduct_en
#ifndef CONF_USB_VENDOR_IPRODUCT_EN
#define CONF_USB_VENDOR_IPRODUCT_EN 0
#endif

#ifndef CONF_USB_VENDOR_IPRODUCT
#define CONF_USB_VENDOR_IPRODUCT                                                                                       \
	(CONF_USB_VENDOR_IPRODUCT_EN * (CONF_USB_VENDOR_IMANUFACT_EN + CONF_USB_VENDOR_IPRODUCT_EN))
#endif

// <s> Unicode string of iProduct
// <id> usb_vendor_iproduct_str
#ifndef CONF_USB_VENDOR_IPRODUCT_STR
#define CONF_USB_VENDOR_IPRODUCT_STR "Vendor Class Demo"
#endif

#ifndef CONF_USB_VENDOR_IPRODUCT_STR_DESC
#define CONF_USB_VENDOR_IPRODUCT_STR_DESC
#endif

// </e>

// <e> Enable string descriptor of iSerialNum
// <id> usb_vendor_iserialnum_en
#ifndef CONF_USB_VENDOR_ISERIALNUM_EN
#define CONF_USB_VENDOR_ISERIALNUM_EN 0
#endif

#ifndef CONF_USB_VENDOR_ISERIALNUM
#define CONF_USB_VENDOR_ISERIALNUM                                                                                     \
	(CONF_USB_VENDOR_ISERIALNUM_EN                                                                                     \
	 * (CONF_USB_VENDOR_IMANUFACT_EN + CONF_USB_VENDOR_IPRODUCT_EN + CONF_USB_VENDOR_ISERIALNUM_EN))
#endif

// <s> Unicode string of iSerialNum
// <id> usb_vendor_iserialnum_str
#ifndef CONF_USB_VENDOR_ISERIALNUM_STR
#define CONF_USB_VENDOR_ISERIALNUM_STR "123456789ABCDEF"
#endif

#ifndef CONF_USB_VENDOR_ISERIALNUM_STR_DESC
#define CONF_USB_VENDOR_ISERIALNUM_STR_DESC
#endif

// </e>

// </h>

// <h> Vendor Class Configuration Descriptor

// <e> Enable string descriptor of iConfig
// <id> usb_vendor_iconfig_en
#ifndef CONF_USB_VENDOR_ICONFIG_EN
#define CONF_USB_VENDOR_ICONFIG_EN 0
#endif

#ifndef CONF_USB_VENDOR_ICONFIG
#define CONF_USB_VENDOR_ICONFIG                                                                                        \
	(CONF_USB_VENDOR_ICONFIG_EN                                                                                        \
	 * (CONF_USB_VENDOR_IMANUFACT_EN + CONF_USB_VENDOR_IPRODUCT_EN + CONF_USB_VENDOR_ISERIALNUM_EN                     \
	    + CONF_USB_VENDOR_ICONFIG_EN))
#endif

// <s> Unicode string of iConfig
// <id> usb_vendor_iconfig_str
#ifndef CONF_USB_VENDOR_ICONFIG_STR
#define CONF_USB_VENDOR_ICONFIG_STR ""
#endif

#ifndef CONF_USB_VENDOR_ICONFIG_STR_DESC
#define CONF_USB_VENDOR_ICONFIG_STR_DESC
#endif

// </e>

// <o> bmAttributes
// <0x80=> Bus power supply, not support for remote wakeup
// <0xA0=> Bus power supply, support for remote wakeup
// <0xC0=> Self powered, not support for remote wakeup
// <0xE0=> Self powered, support for remote wakeup
// <id> usb_vendor_bmattri
#ifndef CONF_USB_VENDOR_BMATTRI
#define CONF_USB_VENDOR_BMATTRI 0x80
#endif

// <o> bMaxPower <0x00-0xFF>
// <id> usb_vendor_bmaxpower
#ifndef CONF_USB_VENDOR_BMAXPOWER
#define CONF_USB_VENDOR_BMAXPOWER 0x32
#endif
// </h>

// <h> Vendor Interface Descriptor

// <o> bInterfaceNumber <0x00-0xFF>
// <id> usb_vendor_bifcnum
#ifndef CONF_USB_VENDOR_BIFCNUM
#define CONF_USB_VENDOR_BIFCNUM 0x0
#endif

// <o> iInterface <0x00-0xFF>
// <id> usb_vendor_iifc
#ifndef CONF_USB_VENDOR_IIFC
#define CONF_USB_VENDOR_IIFC 0x0
#endif

// <o> INTERRUPT IN Endpoint Address
// <0x81=> EndpointAddress = 0x81
// <0x82=> EndpointAddress = 0x82
// <0x83=> EndpointAddress = 0x83
// <0x84=> EndpointAddress = 0x84
// <0x85=> EndpointAddress = 0x85
// <0x86=> EndpointAddress = 0x86
// <0x87=> EndpointAddress = 0x87
// <id> usb_vendor_intin_epaddr
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_INTIN_EPADDR
#define CONF_USB_VENDOR_INTIN_EPADDR 0x81
#endif

// <o> INTERRUPT IN Endpoint wMaxPacketSize
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_vendor_intin_maxpksz
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_INTIN_MAXPKSZ
#define CONF_USB_VENDOR_INTIN_MAXPKSZ 0x40
#endif

// <o> INTERRUPT OUT Endpoint Address
// <0x01=> EndpointAddress = 0x01
// <0x02=> EndpointAddress = 0x02
// <0x03=> EndpointAddress = 0x03
// <0x04=> EndpointAddress = 0x04
// <0x05=> EndpointAddress = 0x05
// <0x06=> EndpointAddress = 0x06
// <0x07=> EndpointAddress = 0x07
// <id> usb_vendor_intout_epaddr
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_INTOUT_EPADDR
#define CONF_USB_VENDOR_INTOUT_EPADDR 0x2
#endif

// <o> INTERRUPT OUT Endpoint wMaxPacketSize
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_vendor_intout_maxpksz
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_INTOUT_MAXPKSZ
#define CONF_USB_VENDOR_INTOUT_MAXPKSZ 0x40
#endif

// <o> BULK IN Endpoint Address
// <0x81=> EndpointAddress = 0x81
// <0x82=> EndpointAddress = 0x82
// <0x83=> EndpointAddress = 0x83
// <0x84=> EndpointAddress = 0x84
// <0x85=> EndpointAddress = 0x85
// <0x86=> EndpointAddress = 0x86
// <0x87=> EndpointAddress = 0x87
// <id> usb_vendor_bulkin_epaddr
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_BULKIN_EPADDR
#define CONF_USB_VENDOR_BULKIN_EPADDR 0x83
#endif

// <o> BULK IN Endpoint wMaxPacketSize
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_vendor_bulkin_maxpksz
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_BULKIN_MAXPKSZ
#define CONF_USB_VENDOR_BULKIN_MAXPKSZ 0x40
#endif

// <o> BULK OUT Endpoint Address
// <0x01=> EndpointAddress = 0x01
// <0x02=> EndpointAddress = 0x02
// <0x03=> EndpointAddress = 0x03
// <0x04=> EndpointAddress = 0x04
// <0x05=> EndpointAddress = 0x05
// <0x06=> EndpointAddress = 0x06
// <0x07=> EndpointAddress = 0x07
// <id> usb_vendor_bulkout_epaddr
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_BULKOUT_EPADDR
#define CONF_USB_VENDOR_BULKOUT_EPADDR 0x4
#endif

// <o> BULK OUT Endpoint wMaxPacketSize
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_vendor_bulkout_maxpksz
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_BULKOUT_MAXPKSZ
#define CONF_USB_VENDOR_BULKOUT_MAXPKSZ 0x40
#endif

// <o> ISOCHRONOUS IN Endpoint Address
// <0x81=> EndpointAddress = 0x81
// <0x82=> EndpointAddress = 0x82
// <0x83=> EndpointAddress = 0x83
// <0x84=> EndpointAddress = 0x84
// <0x85=> EndpointAddress = 0x85
// <0x86=> EndpointAddress = 0x86
// <0x87=> EndpointAddress = 0x87
// <id> usb_vendor_isoin_epaddr
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_ISOIN_EPADDR
#define CONF_USB_VENDOR_ISOIN_EPADDR 0x85
#endif

// <o> ISOCHRONOUS IN Endpoint wMaxPacketSize
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <0x0080=> 128 bytes
// <0x0100=> 256 bytes
// <id> usb_vendor_isoin_maxpksz
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_ISOIN_MAXPKSZ
#define CONF_USB_VENDOR_ISOIN_MAXPKSZ 0x100
#endif

// <o> ISOCHRONOUS OUT Endpoint Address
// <0x01=> EndpointAddress = 0x01
// <0x02=> EndpointAddress = 0x02
// <0x03=> EndpointAddress = 0x03
// <0x04=> EndpointAddress = 0x04
// <0x05=> EndpointAddress = 0x05
// <0x06=> EndpointAddress = 0x06
// <0x07=> EndpointAddress = 0x07
// <id> usb_vendor_isoout_epaddr
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_ISOOUT_EPADDR
#define CONF_USB_VENDOR_ISOOUT_EPADDR 0x6
#endif

// <o> ISOCHRONOUS OUT Endpoint wMaxPacketSize
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <0x0080=> 128 bytes
// <0x0100=> 256 bytes
// <id> usb_vendor_isoout_maxpksz
// <i> Please make sure that the setting here is coincide with the endpoint setting in USB device driver.
#ifndef CONF_USB_VENDOR_ISOOUT_MAXPKSZ
#define CONF_USB_VENDOR_ISOOUT_MAXPKSZ 0x100
#endif
// </h>

// <<< end of configuration section >>>

#endif // USBD_CONFIG_H
