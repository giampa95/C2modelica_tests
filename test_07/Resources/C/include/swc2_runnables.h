#ifndef SWC2_RUNNABLES_H
#define SWC2_RUNNABLES_H

/*
 * swc2_runnables.h
 * ----------------
 * Runnable entities of Software Component SWC2.
 *
 * SWC2's internal ("Per-Instance Memory") variable - SWC2_counter - is
 * file-static in swc2_runnables.c and is not exposed here: only the
 * runnables themselves are part of SWC2's public interface towards the
 * scheduler.
 */

void SWC2_Count(void);
void SWC2_ResetCounter(void);

/* Additional functions to support state management for solver-rollback. */
#include "datatypes.h"
void GetState_SWC2_counter(uint8 *counter);
void SetState_SWC2_counter(const uint8 counter);

#endif /* SWC2_RUNNABLES_H */
