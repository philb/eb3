#include <atmel_start.h>
#include <hal_gpio.h>

#define PB03 GPIO(GPIO_PORTB, 3)
#define PB05 GPIO(GPIO_PORTB, 5)
#define PB14 GPIO(GPIO_PORTB, 14)
#define PB15 GPIO(GPIO_PORTB, 15)
#define PB16 GPIO(GPIO_PORTB, 16)
#define PB17 GPIO(GPIO_PORTB, 17)
#define PB18 GPIO(GPIO_PORTB, 18)
#define PB19 GPIO(GPIO_PORTB, 19)
#define PB20 GPIO(GPIO_PORTB, 20)
#define PB21 GPIO(GPIO_PORTB, 21)

#define PA02 GPIO(GPIO_PORTA, 2)
#define PA14 GPIO(GPIO_PORTA, 14)
#define PA16 GPIO(GPIO_PORTA, 16)
#define PA20 GPIO(GPIO_PORTA, 20)
#define PA21 GPIO(GPIO_PORTA, 21)
#define PA22 GPIO(GPIO_PORTA, 22)

#define PC00 GPIO(GPIO_PORTC, 0)
#define PC01 GPIO(GPIO_PORTC, 1)
#define PC02 GPIO(GPIO_PORTC, 2)
#define PC24 GPIO(GPIO_PORTC, 24)

#define FPGA_SOFT_RESET	PB16
#define FPGA_RX_ENABLE  PB18
#define FPGA_TX_GO      PB19
#define FPGA_FLAG_FILL  PA22

#define FPGA_IDLE	PA20
#define FPGA_NO_CLOCK	PA21

#define LED_SD		PA02
#define LED_ECONET	PA01
#define LED_ONLINE	PA00
#define LED_ERROR	PC00
#define LED_ETHERNET	PC01
#define LED_NO_CLOCK	PC02

static const uint8_t fpga_image[] = {
#include "gcc/fpga_image.h"
};

void msleep(int ms)
{
	volatile int i;
	for (i = 0; i < 10 * ms; i++)
		;
}

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

void load_fpga()
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

	load_fpga();

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
