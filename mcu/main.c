#include <atmel_start.h>
#include <hal_gpio.h>

#include "gpios.h"
#include "fpga.h"
#include "debug.h"
#include "timers.h"

void die()
{
	gpio_set_pin_level(LED_ERROR, true);
	while (1)
		;
}

void lamp_test()
{
	const uint8_t leds[] = { LED_ONLINE, LED_ETHERNET, LED_ECONET, LED_SD, LED_NO_CLOCK, LED_ERROR };
	uint8_t brightness[6] = { 0, 0, 0, 0, 0, 0 };
	int i;
	for (i = 0; i < sizeof(leds); i++) {
		gpio_set_pin_level(leds[i], false);
		gpio_set_pin_direction(leds[i], GPIO_DIRECTION_OUT);
		gpio_set_pin_pull_mode(leds[i], GPIO_PULL_OFF);
	}

	int state = 0;
	while (state < 6) {
		int pwm;
		for (pwm = 0; pwm < 31; pwm++) {
			for (i = 0; i < sizeof(leds); i++) {
				gpio_set_pin_level(leds[i], (brightness[i] > pwm) ? true : false);
				msleep(10);
			}
		}
		switch (state) {
		case 0:
			brightness[2]++;
			brightness[3]++;
			if (brightness[2] == 32)
				state++;
			break;
		case 1:
			brightness[2]--;
			brightness[3]--;
			brightness[1]++;
			brightness[4]++;
			if (brightness[1] == 32)
				state++;
			break;
		case 2:
			brightness[1]--;
			brightness[4]--;
			brightness[0]++;
			brightness[5]++;
			if (brightness[0] == 32)
				state++;
			break;
		case 3:
			brightness[1]++;
			brightness[4]++;
			brightness[0]--;
			brightness[5]--;
			if (brightness[1] == 32)
				state++;
			break;
		case 4:
			brightness[1]--;
			brightness[4]--;
			brightness[2]++;
			brightness[3]++;
			if (brightness[2] == 32)
				state++;
			break;
		case 5:
			brightness[2]--;
			brightness[3]--;
			if (brightness[2] == 0)
				state++;
			break;
		}
	}
}

int main(void)
{
	// Turn error LED on before doing anything else in case we get stuck
	volatile int *pcdd = (volatile int *)0x41008100;
	volatile int *pcdr = (volatile int *)0x41008110;

	*pcdd = 1;
	*pcdr = 1;

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	// CLOCKOUT_EN
	gpio_set_pin_level(PB03, false);
	gpio_set_pin_direction(PB03, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode(PB03, GPIO_PULL_OFF);

	// PHY_RESET#
	gpio_set_pin_level(PB05, false);
	gpio_set_pin_direction(PB05, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode(PB05, GPIO_PULL_OFF);

	// FPGA_SS#
	gpio_set_pin_function(PB14, 0);
	gpio_set_pin_level(PB14, false);
	gpio_set_pin_direction(PB14, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode(PB14, GPIO_PULL_OFF);

	// FPGA_CRESET#
	gpio_set_pin_level(PB20, false);
	gpio_set_pin_direction(PB20, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode(PB20, GPIO_PULL_OFF);

	// FPGA_CDONE#
	gpio_set_pin_direction(PB21, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(PB21, GPIO_PULL_UP);

	// PHY_CLK
	gpio_set_pin_function(PA16, PINMUX_PA16M_GCLK_IO2);
	gpio_set_pin_pull_mode(PA16, GPIO_PULL_OFF);

	// FPGA_CLK
	gpio_set_pin_function(PA14, PINMUX_PA14M_GCLK_IO0);
	gpio_set_pin_pull_mode(PA14, GPIO_PULL_OFF);

	// FPGA soft reset
	gpio_set_pin_function(FPGA_SOFT_RESET, 0);
	gpio_set_pin_level(FPGA_SOFT_RESET, 1);
	gpio_set_pin_direction(FPGA_SOFT_RESET, GPIO_DIRECTION_OUT);

	// FPGA rx enable
	gpio_set_pin_function(FPGA_RX_ENABLE, 0);
	gpio_set_pin_level(FPGA_RX_ENABLE, 0);
	gpio_set_pin_direction(FPGA_RX_ENABLE, GPIO_DIRECTION_OUT);

	// flag fill
	gpio_set_pin_function(FPGA_FLAG_FILL, 0);
	gpio_set_pin_level(FPGA_FLAG_FILL, 0);
	gpio_set_pin_direction(FPGA_FLAG_FILL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(FPGA_TX_GO, 0);
	gpio_set_pin_level(FPGA_TX_GO, 0);
	gpio_set_pin_direction(FPGA_TX_GO, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(FPGA_NO_CLOCK, 0);
	gpio_set_pin_direction(FPGA_NO_CLOCK, GPIO_DIRECTION_IN);

	gpio_set_pin_function(FPGA_IDLE, 0);
	gpio_set_pin_direction(FPGA_IDLE, GPIO_DIRECTION_IN);

	lamp_test();

	gpio_set_pin_level(PB05, true);

	fpga_load();

	//vendor_example();

	gpio_set_pin_level(FPGA_SOFT_RESET, 0);

	spi_s_sync_enable(&SPI_1);

	gpio_set_pin_level(FPGA_RX_ENABLE, 1);

	//gpio_set_pin_direction(PC24, GPIO_DIRECTION_IN);

#if 0
	while (1) {
		if (gpio_get_pin_level(FPGA_NO_CLOCK))
			debug_putc('C');
		if (gpio_get_pin_level(FPGA_IDLE) == 0)
			debug_putc('I');
		if (gpio_get_pin_level(PC24) == 0)
			debug_putc('S');
	}
#endif

	while (1) {
		struct spi_xfer msg;
		uint8_t buf[32];
		memset(&msg, 0, sizeof(msg));
		msg.txbuf = NULL;
		msg.rxbuf = buf;
		msg.size = sizeof(buf);
		int n = spi_s_sync_transfer(&SPI_1, &msg);
		debug_puts("Rx ");
		debug_put_hex(n);
		debug_putc(' ');
		for (int i = 0; i < n; i++)
			debug_put_hex(buf[i]);
		debug_putc('\n');
	}
}
