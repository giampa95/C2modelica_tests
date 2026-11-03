#ifndef SCHEDULER_MANAGER_H
#define SCHEDULER_MANAGER_H

/*
 * scheduler_manager.h
 * --------------------
 * Minimal run-to-completion "runnable stack": collects the runnables that
 * must execute on the current App SW reactivation (pushRunnable, called
 * from the Mng_event* functions in scheduler_events.c), then runs them all
 * (runRunnables, called once per doStep() from appsw.c), and can be
 * emptied without running it (resetScheduler).
 *
 * This is a drastically simplified stand-in for the OS/RTE scheduling
 * machinery of a real AUTOSAR Classic ECU (OS tasks, alarms, schedule
 * tables, RTE-generated runnable activation, possibly multiple cores/
 * partitions, ...): everything here executes synchronously, within a
 * single call to doStep().
 */

#include "datatypes.h"

typedef void (*RunnableType)(void);

/* Maximum number of runnables that can be pending at once. Large enough to
 * hold every runnable used in this example simultaneously (worst case:
 * eventR + eventG + eventClockA all active on the same reactivation). */
#define SCHEDULER_MAX_RUNNABLES 8U

void resetScheduler(void);
void pushRunnable(RunnableType runnable);
void runRunnables(void);

#endif /* SCHEDULER_MANAGER_H */
