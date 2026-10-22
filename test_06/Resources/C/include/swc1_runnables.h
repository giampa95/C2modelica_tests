#ifndef SWC1_RUNNABLES_H
#define SWC1_RUNNABLES_H

/*
 * swc1_runnables.h
 * ----------------
 * Runnable entities of Software Component SWC1.
 *
 * SWC1's internal ("Per-Instance Memory", in AUTOSAR terms) variables -
 * SWC1_value, SWC1_gain, SWC1_valueGained - are file-static in
 * swc1_runnables.c and are not exposed here: only the runnables
 * themselves are part of SWC1's public interface towards the scheduler.
 */

void SWC1_ComputeValueGained(void);
void SWC1_SaveGain(void);
void SWC1_ResetGain(void);

/* Additional functions to support state management for solver-rollback. */
#include "datatypes.h"
void GetState_SWC1_value(int32 *value);
void SetState_SWC1_value(const int32 value);
void GetState_SWC1_gain(int32 *gain);
void SetState_SWC1_gain(const int32 gain);
void GetState_SWC1_valueGained(int32 *valueGained);
void SetState_SWC1_valueGained(const int32 valueGained);

#endif /* SWC1_RUNNABLES_H */
