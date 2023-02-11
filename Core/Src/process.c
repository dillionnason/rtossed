#include "process.h"
#include <stdint.h>
#include <string.h>

#define PROC_MAX 4

struct task_struct process_table[PROC_MAX];
struct task_struct idle_task;
struct task_struct *current = &idle_task;

/*
 *  Intialize the user space stack
 */
void process_stack_init(struct task_struct *task) 
{
	/* get the stack pointer from the task structure */
	uint32_t *psp = (uint32_t *)task->r.SP;

	/* move the stack pointer down by 18 and clear FPSCR - S0 */
	psp -= 18; 
	memset(psp, 0, sizeof(uint32_t) * 17);

	/* store task registers on stack */
	*(psp--) = task->r.xPSR;
	*(psp--) = task->r.PC;
	*(psp--) = task->r.LR;
	*(psp--) = task->r.R12;
	*(psp--) = task->r.R3;
	*(psp--) = task->r.R2;
	*(psp--) = task->r.R1;
	*(psp--) = task->r.R0;

	task->r.SP = (uint32_t)psp;
}
