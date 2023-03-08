#include "process.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

/*
 *  Process sleep in microseconds
 */
int microsleep(int useconds) 
{
	current->w_time = (uwTick * 1000) + useconds;
	current->state &= ~(STATE_RUN);
	current->state |= STATE_TIME_SLEEP;
	yield();
}
