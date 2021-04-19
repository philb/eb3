#include <atmel_start.h>
#include <hal_gpio.h>

#define PB03 GPIO(GPIO_PORTB, 3)
#define PB05 GPIO(GPIO_PORTB, 5)
#define PB14 GPIO(GPIO_PORTB, 14)
#define PB20 GPIO(GPIO_PORTB, 20)
#define PB21 GPIO(GPIO_PORTB, 21)

#define PA14 GPIO(GPIO_PORTA, 14)
#define PA16 GPIO(GPIO_PORTA, 16)

int main(void)
{
	volatile int *p = 0x41008000;
	volatile int *p2 = 0x41008010;

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	*p = 7;
	*p2 = 7;
  
	usb_d_enable();
	//usb_d_attach();

	// CLOCKOUT_EN
	gpio_set_pin_level(PB03, false);
	gpio_set_pin_direction(PB03, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode(PB03, GPIO_PULL_OFF);

	// PHY_RESET#
	gpio_set_pin_level(PB05, false);
	gpio_set_pin_direction(PB05, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode(PB05, GPIO_PULL_OFF);

	// FPGA_SS#
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
	
	debug_puts("Hello");
	
	gpio_set_pin_level(PB05, true);
	
	/* Replace with your application code */
	while (1) {
	}
}
