/*
 * 4.4 System timer, SysTick
 * The processor has a 24-bit system timer, SysTick, that counts down from the reload value to
 * zero, reloads, that is wraps to, the value in the SYST_RVR register on the next clock edge, then
 * counts down on subsequent clocks.
 *
 */


#include "timebase.h"
#include "stm32f4xx.h"

#define ONE_SEC_LOAD		16000000	// 1 second cycle

#define CTRL_ENABLE			(1U<<0)
#define CTRL_TICKINT		(1U<<1)
#define CTRL_CLCKSRC		(1U<<2)
#define CTRL_COUNTFLAG		(1U<<16)

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;
volatile uint32_t tick_freq = 1;

/**
 * Sets the internal clock for our system.
 * No parameters.
 * No return values.
 *
 * Steps:
 * 	1 - Reload the time with number of cylces per second.
 * 	2 - Declare systick current value register.
 * 	3 - Select internal clock source,
 * 	4 - Enable interrupt.
 * 	5 - Enable systick.
 * 	6 - Enable global interrupts.
 */
void timebase_init(void)
{
	// (1)
	SysTick->LOAD = ONE_SEC_LOAD - 1;
	// (2)
	SysTick->VAL = 0;
	// (3)
	SysTick->CTRL = CTRL_CLCKSRC;
	// (4)
	SysTick->CTRL |= CTRL_TICKINT;
	// (5)
	SysTick->CTRL |= CTRL_ENABLE;
	// (6)
	__enable_irq();
}








