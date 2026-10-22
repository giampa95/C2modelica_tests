/*
 * scheduler_manager.c
 * --------------------
 * See scheduler_manager.h for the design rationale.
 *
 * Despite being called a "stack" (per the naming convention requested for
 * this module - pushRunnable / resetScheduler), pending runnables are
 * stored and executed in the order they were pushed (index 0 upward),
 * i.e. as a FIFO queue rather than a LIFO stack.
 *
 * This is deliberate, and is what makes it simple to honor the event
 * priority (eventR > eventG > eventClockA): doStep() (appsw.c) calls the
 * Mng_event* functions in that same priority order, so the runnables of
 * the highest-priority *active* event are always pushed - and therefore
 * executed - first.
 */

#include "scheduler_manager.h"
#include <stddef.h>

static RunnableType s_runnableQueue[SCHEDULER_MAX_RUNNABLES];
static uint8 s_runnableCount = 0U;

void resetScheduler(void)
{
    s_runnableCount = 0U;
}

void pushRunnable(RunnableType runnable)
{
    if ((runnable != NULL) && (s_runnableCount < SCHEDULER_MAX_RUNNABLES))
    {
        s_runnableQueue[s_runnableCount] = runnable;
        s_runnableCount++;
    }
    /* A real BSW would raise a DET / scheduling error on overflow instead
     * of silently dropping the runnable. */
}

void runRunnables(void)
{
    uint8 i;

    for (i = 0U; i < s_runnableCount; i++)
    {
        s_runnableQueue[i]();
    }

    resetScheduler();
}
