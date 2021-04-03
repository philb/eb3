/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include "usb_start.h"

static uint8_t single_desc_bytes[] = {
    /* Device descriptors and Configuration descriptors list. */
    VENDOR_DESCES_LS_FS};

static struct usbd_descriptors single_desc[] = {{single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)}
#if CONF_USBD_HS_SP
                                                ,
                                                {NULL, NULL}
#endif
};

/** Ctrl endpoint buffer */
static uint8_t ctrl_buffer[64];

/** Loop back buffer for data validation */
COMPILER_ALIGNED(4)
static uint8_t main_buf_loopback[1024];

/**
 * \brief Ctrl endpointer out transfer callback function
 * \param[in] count data amount to transfer
 */
static void usb_device_cb_ctrl_out(uint16_t count)
{
	uint16_t recv_cnt = 1024;

	if (recv_cnt > count) {
		recv_cnt = count;
	}
	vendordf_read(USB_EP_TYPE_CONTROL, main_buf_loopback, recv_cnt);
	vendordf_read(USB_EP_TYPE_INTERRUPT, main_buf_loopback, 1024);
	vendordf_read(USB_EP_TYPE_BULK, main_buf_loopback, 1024);
	vendordf_read(USB_EP_TYPE_ISOCHRONOUS, main_buf_loopback, 1024);
}

/**
 * \brief Ctrl endpointer in transfer callback function
 * \param[in] count data amount to transfer
 */
static void usb_device_cb_ctrl_in(uint16_t count)
{
	uint16_t recv_cnt = 1024;

	if (recv_cnt > count) {
		recv_cnt = count;
	}
	vendordf_write(USB_EP_TYPE_CONTROL, main_buf_loopback, recv_cnt);
}

/**
 * \brief Interrupt endpointer out transfer callback function
 * \param[in] ep Endpointer number
 * \param[in] rc Transfer status codes
 * \param[in] count Data amount to transfer
 * \return Operation status.
 */
static bool usb_device_cb_int_out(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	vendordf_write(USB_EP_TYPE_INTERRUPT, main_buf_loopback, count);
	return false;
}

/**
 * \brief Interrupt endpointer in transfer callback function
 * \param[in] ep Endpointer number
 * \param[in] rc Transfer status codes
 * \param[in] count Data amount to transfer
 * \return Operation status.
 */
static bool usb_device_cb_int_in(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	vendordf_read(USB_EP_TYPE_INTERRUPT, main_buf_loopback, count);
	return false;
}

/**
 * \brief Bulk endpointer out transfer callback function
 * \param[in] ep Endpointer number
 * \param[in] rc Transfer status codes
 * \param[in] count Data amount to transfer
 * \return Operation status.
 */
static bool usb_device_cb_bulk_out(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	vendordf_write(USB_EP_TYPE_BULK, main_buf_loopback, count);

	return false;
}

/**
 * \brief Bulk endpointer in transfer callback function
 * \param[in] ep Endpointer number
 * \param[in] rc Transfer status codes
 * \param[in] count Data amount to transfer
 * \return Operation status.
 */
static bool usb_device_cb_bulk_in(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	vendordf_read(USB_EP_TYPE_BULK, main_buf_loopback, count);

	return false;
}

/**
 * \brief Isochronous endpointer out transfer callback function
 * \param[in] ep Endpointer number
 * \param[in] rc Transfer status codes
 * \param[in] count Data amount to transfer
 * \return Operation status.
 */
static bool usb_device_cb_iso_out(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	vendordf_write(USB_EP_TYPE_ISOCHRONOUS, main_buf_loopback, count);

	return false;
}

/**
 * \brief Isochronous endpointer in transfer callback function
 * \param[in] ep Endpointer number
 * \param[in] rc Transfer status codes
 * \param[in] count Data amount to transfer
 * \return Operation status.
 */
static bool usb_device_cb_iso_in(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	vendordf_read(USB_EP_TYPE_ISOCHRONOUS, main_buf_loopback, count);

	return false;
}

/**
 * Example of Vendor Function.
 * \note
 * In this example, we will use a PC as a USB host:
 * - Connect the DEBUG USB on XPLAINED board to PC for program download.
 * - Connect the TARGET USB on XPLAINED board to PC for running program.
 * The example uses a vendor class which implements a loopback on the following
 * endpoints types: control, interrupt, bulk and isochronous.
 * The host application developed on libusb library is provided with application
 * note AVR4901. Except for control endpoint, other type endpoints are optional.
 * Application always run from control endpoint firstly.
 */
void vendor_example(void)
{
	/* usb stack init */
	usbdc_init(ctrl_buffer);

	/* usbdc_register_funcion inside */
	vendordf_init();
	vendordf_register_callback(VENDORDF_CB_CTRL_READ_REQ, (FUNC_PTR)usb_device_cb_ctrl_out);
	vendordf_register_callback(VENDORDF_CB_CTRL_WRITE_REQ, (FUNC_PTR)usb_device_cb_ctrl_in);
	vendordf_register_callback(VENDORDF_CB_INT_READ, (FUNC_PTR)usb_device_cb_int_out);
	vendordf_register_callback(VENDORDF_CB_INT_WRITE, (FUNC_PTR)usb_device_cb_int_in);
	vendordf_register_callback(VENDORDF_CB_BULK_READ, (FUNC_PTR)usb_device_cb_bulk_out);
	vendordf_register_callback(VENDORDF_CB_BULK_WRITE, (FUNC_PTR)usb_device_cb_bulk_in);
	vendordf_register_callback(VENDORDF_CB_ISO_READ, (FUNC_PTR)usb_device_cb_iso_out);
	vendordf_register_callback(VENDORDF_CB_ISO_WRITE, (FUNC_PTR)usb_device_cb_iso_in);

	usbdc_start(single_desc);
	usbdc_attach();

	while (!vendordf_is_enabled()) {
		// wait vendor application to be installed
	};
}

void usb_init(void)
{
}
