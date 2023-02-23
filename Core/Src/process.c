#include "process.h"
#include "sh.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */
#define PROC_MAX 4

/* global variables */
struct task_struct process_table[PROC_MAX];
struct task_struct idle_task;
struct task_struct *current = &idle_task;

/*
 *  Process scheduler
 */
struct task_struct *schedule(void)
{
	if (current == &idle_task) {
		return &process_table[0];
	}
	return &idle_task;
}

/*
 *  Start the current process
 *  This function should not return, if the process returns it will go 
 *  into a while loop until the scheduler stops the process. 
 */
void process_start(void) 
{
	current->cmd();
	current->state = STATE_STOP;
	while (1) {}
}

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

	/* push task registers on stack */
	*(--psp) = task->r.xPSR;
	*(--psp) = task->r.PC;
	*(--psp) = task->r.LR;
	*(--psp) = task->r.R12;
	*(--psp) = task->r.R3;
	*(--psp) = task->r.R2;
	*(--psp) = task->r.R1;
	*(--psp) = task->r.R0;

	task->r.SP = (uint32_t)psp;
}

/*
 *  Initialize the process table
 */
void process_table_init(void)
{
	/* clear the process table */
	memset(&process_table, 0, sizeof process_table);

	/* set up the shell task structure */
	process_table[0].r.SP			  = __get_PSP();
	process_table[0].sp_start   = __get_PSP();
	process_table[0].r.LR			  = 0;
	process_table[0].r.PC			  = (uint32_t)&process_start;
	process_table[0].r.xPSR		  = 0x01000000;  // thumb state
	process_table[0].state      = STATE_RUN;
	process_table[0].cmd	      = sh;
	process_table[0].exc_return = EXC_RETURN_THREAD_PSP;
	process_table[0].pid	      = 0;
	process_stack_init(&process_table[0]);

	/* set up the idle task structure */
	idle_task.state      = STATE_STOP;
	idle_task.r.xPSR     = 0x01000000;
	idle_task.exc_return = EXC_RETURN_THREAD_MSP_FPU;
	idle_task.pid	       = -2;
}
