/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include <atmel_start.h>
#include <tcpip_lite_start.h>

#include <sys/time.h>

static uint32_t   ms_tick = 0;
struct timer_task ms_tick_task;

static void tcpip_lite_timer_task_cb(const struct timer_task *const t)
{
	(void)t;
	ms_tick++;
}

int _gettimeofday(struct timeval *tv, void *tzvp)
{
	if (!tv)
		return -1;

	tv->tv_sec  = ms_tick / 1000;
	tv->tv_usec = ms_tick * 1000;

	return 0;
}

void tcpip_lite_stack_init(void)
{

	Network_Init();
	SYSLOG_Init();

	/* Start Timer Task */

	ms_tick_task.interval = 1;
	ms_tick_task.cb       = tcpip_lite_timer_task_cb;
	ms_tick_task.mode     = TIMER_TASK_REPEAT;
	timer_add_task(&Timer, &ms_tick_task);
	timer_start(&Timer);
}
