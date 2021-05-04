#include <atmel_start.h>
#include "timers.h"

static volatile uint32_t ms_tick;
struct timer_task ms_tick_task;

static void timer_task_cb(const struct timer_task *const t)
{
	(void)t;
	ms_tick++;
}

void tick_init(void)
{
	ms_tick_task.interval = 1;
	ms_tick_task.cb       = timer_task_cb;
	ms_tick_task.mode     = TIMER_TASK_REPEAT;
	timer_add_task(&Timer, &ms_tick_task);
	timer_start(&Timer);
}

uint32_t get_time_ms()
{
	return ms_tick;
}

void msleep(int ms)
{
	uint32_t start = ms_tick;
	while ((ms_tick - start) < ms)
		;
}
