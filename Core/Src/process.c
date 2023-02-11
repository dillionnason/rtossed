#include "process.h"

#define PROC_MAX 4

struct task_struct process_table[PROC_MAX];
struct task_struct idle_task;
struct task_struct *current = &idle_task;
