
#include "clock.h"
#include "compiler.h"

volatile clock_time_t timerCounter;

/**
 * Initialize the clock library.
 *
 * This function initializes the clock library and should be called
 * from the main() function of the system.
 *
 */
void clock_init(void)
{

	// set the clock timer
	timerCounter = 0;

	// set up timer 3 to generate timer clock output
//	TCCR3A = ((1 << WGM32));			// TCCR3A - Timer/Counter3 Control Register A
	// this gives input frequency of 31250Hz from an 8 MHz clock
	TCCR3B = (1 << CS32);						// prescaler/256

	// timer3 overflow interrupt enable
	ETIMSK = (1 << OCIE3A );

	// set counter OCR3A to 31248 0x7A10
	OCR3AH = 0x7A;
	OCR3AL = 0x10;

	sei();

	return;
}

#ifdef __IMAGECRAFT__
#pragma interrupt_handler TIMER3_COMPA_vect:iv_TIMER3_COMPA
#endif


ISR(TIMER3_COMPA_vect)
{
	timerCounter++;

	// zero the counter
	TCNT3H = 0;
	TCNT3L = 0;

	// reset interrupt flag
	ETIFR = 0;
}


/**
 * Get the current clock time.
 *
 * This function returns the current system clock time.
 *
 * \return The current clock time, measured in system ticks.
 */
clock_time_t clock_time(void)
{
	return timerCounter;
}
