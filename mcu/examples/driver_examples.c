/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using DIGITAL_GLUE_LOGIC_0.
 */
void DIGITAL_GLUE_LOGIC_0_example(void)
{
	custom_logic_enable();
	/* Customer logic now works. */
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{
}

static uint8_t src_data[512];
static uint8_t chk_data[512];
/**
 * Example of using FLASH_0 to read and write Flash main array.
 */
void FLASH_0_example(void)
{
	uint32_t page_size;
	uint16_t i;

	/* Init source data */
	page_size = flash_get_page_size(&FLASH_0);

	for (i = 0; i < page_size; i++) {
		src_data[i] = i;
	}

	/* Write data to flash */
	flash_write(&FLASH_0, 0x3200, src_data, page_size);

	/* Read data from flash */
	flash_read(&FLASH_0, 0x3200, chk_data, page_size);
}

/**
 * Lock GCLK.
 */
void pac_example(void)
{
	bool stat;

	periph_get_lock_state(GCLK, &stat);
	if (!stat) {
		periph_lock(GCLK);
	}
}

static struct timer_task TIMER_0_task1, TIMER_0_task2;
/**
 * Example of using TIMER_0.
 */
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_0_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_0_example(void)
{
	TIMER_0_task1.interval = 100;
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 200;
	TIMER_0_task2.cb       = TIMER_0_task2_cb;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
}

/**
 * Example of using USART_0 to write "Hello World" using the IO abstraction.
 */
void USART_0_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using SPI_0 to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_0[12] = "Hello World!";

void SPI_0_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_0, &io);

	spi_m_sync_enable(&SPI_0);
	io_write(io, example_SPI_0, 12);
}

/**
 * Example of using SPI_1 to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_1[12] = "Hello World!";

void SPI_1_example(void)
{
	struct io_descriptor *io;
	spi_s_sync_get_io_descriptor(&SPI_1, &io);

	spi_s_sync_enable(&SPI_1);
	io_write(io, example_SPI_1, 12);
}

static struct timer_task Timer_task1, Timer_task2;

/**
 * Example of using Timer.
 */
static void Timer_task1_cb(const struct timer_task *const timer_task)
{
}

static void Timer_task2_cb(const struct timer_task *const timer_task)
{
}

void Timer_example(void)
{
	Timer_task1.interval = 100;
	Timer_task1.cb       = Timer_task1_cb;
	Timer_task1.mode     = TIMER_TASK_REPEAT;
	Timer_task2.interval = 200;
	Timer_task2.cb       = Timer_task2_cb;
	Timer_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&Timer, &Timer_task1);
	timer_add_task(&Timer, &Timer_task2);
	timer_start(&Timer);
}

/**
 * Example of using WDT_0.
 */
void WDT_0_example(void)
{
	uint32_t clk_rate;
	uint16_t timeout_period;

	clk_rate       = 1000;
	timeout_period = 4096;
	wdt_set_timeout_period(&WDT_0, clk_rate, timeout_period);
	wdt_enable(&WDT_0);
}
