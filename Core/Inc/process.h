#ifndef __PROCESS_H
#define __PROCESS_H

#include <main.h>
#include <stdint.h>
#include <sys/types.h>

extern int kready;
extern struct task_struct idle_task;
extern struct task_struct *current;

/*
 * Core Register Structure
 */
struct __attribute__((__packed__)) registers_struct {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R4;
	uint32_t R5;
	uint32_t R6;
	uint32_t R7;
	uint32_t R8;
	uint32_t R9;
	uint32_t R10;
	uint32_t R11;
	uint32_t R12;  // General purpose registers
	uint32_t SP;   // Stack pointer
	uint32_t LR;   // Link register
	uint32_t PC;   // Program counter
	uint32_t xPSR; // Program status register
};

/*
 * Task states
 */
#define PROCESS_UNUSED    0x0000
#define STATE_TIME_SLEEP  (0b1) << 1
#define STATE_IO_SLEEP    (0b1) << 2
#define STATE_RUN         (0b1) << 3
#define STATE_STOP        (0b1) << 4
#define STATE_ZOMBIE      (0b1) << 5

#define IS_RUNNING(x)     (x->state & STATE_RUN) >> 3
#define IS_SLEEPING(x)    (x->state & STATE_TIME_SLEEP) >> 1

/*
 * Task (process) Structure 
 */
struct __attribute__((__packed__)) task_struct {
	uint32_t state;            // State of the process, based on defines above
	pid_t pid;                 // Unique integer to identify process
	uint32_t exc_return;       // Indicates how exception handler returns
	uint32_t sp_start;         // Starting stack pointer address
	int (*cmd)(void);          // Function pointer
	struct registers_struct r; // Structure of saved core registers
	int w_time;                // Wake time
};

/*
 *  Set the PendSV bit and execute data and instructions synchronization barriers
 */
static inline void yield(void)
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	__DSB(); // Data Synchronization Barrier
	__ISB(); // Instruction Synchronization Barrier
}

/*
 *  Push registers to the main stack
 */
static inline void context_register_save(void)
{
	__asm__ __volatile__ (
		"push	{r4-r11}\n\t"
	);
}

/*
 *  Load registers from the process table
 */
static inline void context_register_restore(struct task_struct *next)
{
	__asm__ __volatile__ (
		"ldm	%0, {r4-r11}\n\t"
		: : "r" (&next->r.R4)
	);
}

/*
 *  Load program counter from process table
 */
static inline void context_switch_return(struct task_struct *next)
{
	__asm__ __volatile__ (
		"ldr	pc, %0\n\t"
		: : "m" (next->exc_return)
	);
}

struct task_struct *schedule(void);
void process_stack_init(struct task_struct *task);
void process_table_init(void);
void process_start(void);

#endif // __PROCESS_H
