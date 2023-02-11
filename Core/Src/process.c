#include "process.h"
#include <stdint.h>
#include <string.h>

#define PROC_MAX 4

struct task_struct process_table[PROC_MAX];
struct task_struct idle_task;
struct task_struct *current = &idle_task;

/* User stack */
extern const uint32_t _eustack[];

extern int (*sh)(void);

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

/*
 *  Initialize the process table
 */
void process_table_init(void)
{
	/* clear the process table */
	memset(&process_table, 0, sizeof process_table);

	/* set up the shell task structure */
	struct task_struct *shell_task = &process_table[0];
	shell_task->r.SP			  = _eustack[0];
	shell_task->sp_start    = _eustack[0];
	shell_task->r.LR			  = 0;
	shell_task->r.PC			  = (uint32_t)&process_start;
	shell_task->r.xPSR		  = 0x01000000;  // thumb state
	shell_task->state	      = STATE_RUN;
	shell_task->cmd		      = sh;
	shell_task->exc_return  = EXC_RETURN_THREAD_PSP;
	shell_task->pid		      = 0;
	process_stack_init(shell_task);

	/* set up the idle task structure */
	idle_task.state      = STATE_STOP;
	idle_task.r.xPSR     = 0x01000000;
	idle_task.exc_return = EXC_RETURN_THREAD_MSP_FPU;
	idle_task.pid	       = -2;
}
