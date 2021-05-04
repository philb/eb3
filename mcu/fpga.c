#include <atmel_start.h>
#include <hal_gpio.h>

#include "gpios.h"
#include "fpga.h"
#include "debug.h"
#include "timers.h"

static const uint8_t fpga_image[] = {
#include "gcc/fpga_image.h"
};

void fpga_load()
{
	spi_m_sync_enable(&SPI_0);

	// CRESET high
	gpio_set_pin_level(PB20, true);

	msleep(100);

	struct spi_xfer msg;
	memset(&msg, 0, sizeof(msg));
	msg.txbuf = (uint8_t *)&fpga_image[0];
	msg.size = sizeof(fpga_image);
	int r = spi_m_sync_transfer(&SPI_0, &msg);
	if (r < 0) { debug_puts("SPI fail\n"); die(); }

	if (gpio_get_pin_level(PB21) == 0) { debug_puts("No CDONE\n"); die(); }

	memset(&msg, 0, sizeof(msg));
	msg.txbuf = (uint8_t *)&fpga_image[0];
	msg.size = 8;
	spi_m_sync_transfer(&SPI_0, &msg);
}

void fpga_rx_enable(bool e)
{
	gpio_set_pin_level(FPGA_RX_ENABLE, e);
}

void fpga_tx_go(bool e)
{
	gpio_set_pin_level(FPGA_TX_GO, e);
}
