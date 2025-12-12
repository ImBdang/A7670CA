#include "task_control.h"

current_task_t cur_task = IDLE_TASK;

void set_cur_task(current_task_t task)
{
    cur_task = task;
}

current_task_t get_cur_task(void)
{
    return cur_task;
}

bool task_busy(void)
{
    if (cur_task == IDLE_TASK)
        return true;
    return false;
}

