/*
 * scheduler_events.c
 * -------------------
 * See scheduler_events.h for the design rationale (in particular: why
 * these Mng_event* functions are only a stand-in for real OS/RTE
 * scheduling).
 */

#include "scheduler_events.h"
#include "scheduler_manager.h"
#include "swc1_runnables.h"
#include "swc2_runnables.h"

void Mng_eventClockA(void)
{
    /* Periodic ("time-based") reactivation: compute the gained value and
     * advance the free-running counter. */
    pushRunnable(&SWC1_ComputeValueGained);
    pushRunnable(&SWC2_Count);
    /* Other runnables mapped to eventClockA could be pushed here. */
}

void Mng_eventG(void)
{
    /* "Save gain" (event-based) reactivation. */
    pushRunnable(&SWC1_SaveGain);
    /* Other runnables mapped to eventG could be pushed here. */
}

void Mng_eventR(void)
{
    /* Reset (event-based) reactivation. */
    pushRunnable(&SWC1_ResetGain);
    pushRunnable(&SWC2_ResetCounter);
    /* Other runnables mapped to eventR could be pushed here. */
}
