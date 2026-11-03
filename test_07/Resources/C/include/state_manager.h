#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

/*
 * state_manager.h
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

#include "datatypes.h"

/*
 * Complete App SW state. 
 */
typedef struct
{
    /* Interface globals */
    int32 gSWC1_value;
    int32 gSWC1_gain;
    int32 gSWC1_valueGained;
    uint8 gSWC2_counter;

    /* SWC1 Per-Instance Memory */
    int32 SWC1_value;
    int32 SWC1_gain;
    int32 SWC1_valueGained;

    /* SWC2 Per-Instance Memory */
    uint8 SWC2_counter;

} SWState;

/* Used by the constructor(): record the post-reset state as the rollback reference
 * (used by rollbacks before the first doStep()) and clear the history. */
void StateMng_init(void);

/* Used by destructor(): forget all history. */
void StateMng_reset(void);

/* Used by doStep(), before the reactivation logic: rewind the C state to what
 * it was at simTime, undoing any step already executed there. */
void StateMng_prepareStep(double simTime);

/* Used by doStep(), after the reactivation logic: record the resulting state
 * as the post-state of the instant prepared above. */
void StateMng_commitStep(void);

#endif /* STATE_MANAGER_H */