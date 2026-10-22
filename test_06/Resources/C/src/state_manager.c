/*
 * state_manager.c
 * ---------------
 * 
 * Solver-rollback support for the App SW library.
 *
 * OpenModelica's solver restores every on a rollback, but
 * not the C state hidden in this library: a doStep() re-invoked at an
 * already executed instant would double-apply its effects.
 * To remedy this issue, every doStep() is time-tagged and 
 * the state snapshotted before (pre) and after (post) the step.
 * StateMng_prepareStep() then guarantees that
 * each instant is applied exactly once:
 *   - new instant      : new snapshot pair, execute;
 *   - same instant     : restore the pre-state, re-execute (idempotent);
 *   - earlier instant  : restore the post-state of the newest snapshot
 *                        at/before the target time, forget later steps;
 *   - before all steps : restore the state captured by StateMng_init().
 */

#include "state_manager.h"
#include "state_interfaces.h" /* GetState_ / SetState_ for the globals (defined for solver-rollback support, copy of Read_ / Write_)  */
#include "swc1_runnables.h"   /* GetState_ / SetState_ for SWC1 internals (defined for solver-rollback support) */
#include "swc2_runnables.h"   /* GetState_ / SetState_ for SWC2 internals (defined for solver-rollback support) */
#include <string.h>

/* Recorded instants kept for rollback; the oldest is evicted when full. */
#define STATE_CAPACITY 32

typedef struct
{
    double  simTime; /* Instant this entry was (last) stepped at. */
    SWState pre;     /* State just before that step.             */
    SWState post;    /* State just after that step.              */
} StateMng_Entry;

static SWState        StateMng_initialState;            /* After constructor(). */
static StateMng_Entry StateMng_history[STATE_CAPACITY]; /* Increasing simTime.  */
static int            StateMng_count  = 0;              /* Valid entries.       */
static int            StateMng_active = -1;             /* In-flight entry.     */


/* Capture the snapshot */
static void StateMng_capture(SWState *out)
{
    /* Interface globals */
    GetState_gSWC1_value(&out->gSWC1_value);
    GetState_gSWC1_gain(&out->gSWC1_gain);
    GetState_gSWC1_valueGained(&out->gSWC1_valueGained);
    GetState_gSWC2_counter(&out->gSWC2_counter);

    /* SWC1 Per-Instance Memory */
    GetState_SWC1_value(&out->SWC1_value);
    GetState_SWC1_gain(&out->SWC1_gain);
    GetState_SWC1_valueGained(&out->SWC1_valueGained);

    /* SWC2 Per-Instance Memory */
    GetState_SWC2_counter(&out->SWC2_counter);
}

/* Restore the snapshot */
static void StateMng_restore(const SWState *in)
{
    /* Interface globals */
    SetState_gSWC1_value(in->gSWC1_value);
    SetState_gSWC1_gain(in->gSWC1_gain);
    SetState_gSWC1_valueGained(in->gSWC1_valueGained);
    SetState_gSWC2_counter(in->gSWC2_counter);

    /* SWC1 Per-Instance Memory */
    SetState_SWC1_value(in->SWC1_value);
    SetState_SWC1_gain(in->SWC1_gain);
    SetState_SWC1_valueGained(in->SWC1_valueGained);

    /* SWC2 Per-Instance Memory */
    SetState_SWC2_counter(in->SWC2_counter);
}

/* StateMng_beginEntry - open a history entry for a not-yet-stepped instant. 
 * File-static helper, hence not in the header (which declares only the public API). */
static void StateMng_beginEntry(double simTime)
{
    if (StateMng_count == STATE_CAPACITY)
    {
        memmove(&StateMng_history[0], &StateMng_history[1],
                (size_t)(STATE_CAPACITY - 1) * sizeof(StateMng_Entry));
        StateMng_count--;
    }
    StateMng_history[StateMng_count].simTime = simTime;
    StateMng_capture(&StateMng_history[StateMng_count].pre);
    StateMng_active = StateMng_count;
    StateMng_count++;
}

/* Used by the constructor(): record the post-reset state as the rollback reference
 * (used by rollbacks before the first doStep()) and clear the history. */
void StateMng_init(void)
{
    StateMng_capture(&StateMng_initialState);
    StateMng_count  = 0;
    StateMng_active = -1;
}

/* Used by destructor(): forget all history. */
void StateMng_reset(void)
{
    StateMng_count  = 0;
    StateMng_active = -1;
}

/* Used by doStep(), before the reactivation logic: rewind the C state to what
 * it was at simTime, undoing any step already executed there. */
void StateMng_prepareStep(double simTime)
{
    int i;

    if (StateMng_count == 0)
    {
        /* No step recorded yet: nothing to rewind. */
        StateMng_beginEntry(simTime);
        return;
    }

    if (simTime > StateMng_history[StateMng_count - 1].simTime)
    {
        /* Forward execution: new instant. */
        StateMng_beginEntry(simTime);
        return;
    }

    if (simTime == StateMng_history[StateMng_count - 1].simTime)
    {
        /* Re-invocation of the newest instant: rewind to its pre-state
         * so the reactivation applies exactly once, whatever the new
         * inputs. Entry reused; commitStep() overwrites its post. */
        StateMng_restore(&StateMng_history[StateMng_count - 1].pre);
        StateMng_active = StateMng_count - 1;
        return;
    }

    /* simTime < newest instant: rollback. Find the newest entry
     * at/before simTime. */
    i = StateMng_count - 1;
    while ((i >= 0) && (StateMng_history[i].simTime > simTime))
    {
        i--;
    }

    if (i >= 0)
    {
        if (simTime == StateMng_history[i].simTime)
        {
            /* Onto an executed instant: rewind to its pre-state and
             * re-execute it; later steps are forgotten. */
            StateMng_restore(&StateMng_history[i].pre);
            StateMng_active = i;
            StateMng_count  = i + 1;
            return;
        }

        /* Between two recorded steps: take the post-state of the newest
         * step at/before simTime, forget every later step. */
        StateMng_restore(&StateMng_history[i].post);
        StateMng_count = i + 1;
    }
    else
    {
        /* Behind the first recorded step: back to the post-constructor
         * state. */
        StateMng_restore(&StateMng_initialState);
        StateMng_count = 0;
    }

    /* Fresh instant at/after the rewound point. */
    StateMng_beginEntry(simTime);
}

/* Used by doStep(), after the reactivation logic: record the resulting state
 * as the post-state of the instant prepared above. */
void StateMng_commitStep(void)
{
    if ((StateMng_active >= 0) && (StateMng_active < StateMng_count))
    {
        StateMng_capture(&StateMng_history[StateMng_active].post);
    }
    StateMng_active = -1;
}
