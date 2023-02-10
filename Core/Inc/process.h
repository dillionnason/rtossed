#ifndef __PROCESS_H
#define __PROCESS_H

#include <main.h>

extern int kready;

static inline void yield(void)
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	__DSB();			// Data Synchronization Barrier
	__ISB();			// Instruction Synchronization Barrier
}

#endif // __PROCESS_H
