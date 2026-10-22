/*
 * swc1_runnables.c
 * ----------------
 * Software Component SWC1: internal variables (this SW-C's "Per-Instance
 * Memory") and runnables.
 */

#include "swc1_runnables.h"
#include "interfaces.h"
#include "datatypes.h"

/* SWC1 internal variables. */
static int32 SWC1_value       = 0;
static int32 SWC1_gain        = 1;
static int32 SWC1_valueGained = 0;

/*
 * SWC1_ComputeValueGained  (mapped to eventClockA, periodic)
 * -----------------------------------------------------------
 * Reads the current external value, multiplies it by the gain SWC1
 * currently has stored internally (SWC1_gain - only ever updated by
 * SWC1_SaveGain on eventG, or reset by SWC1_ResetGain on eventR), and
 * publishes the result.
 */
void SWC1_ComputeValueGained(void)
{
    Read_gSWC1_value(&SWC1_value);
    SWC1_valueGained = SWC1_value * SWC1_gain;
    Write_gSWC1_valueGained(SWC1_valueGained);
}

/*
 * SWC1_SaveGain  (mapped to eventG)
 * -----------------------------------
 * "Latches" the gain currently offered on the interface (written on every
 * doStep() call by the environment - see appsw.c) into SWC1's own internal
 * gain, and republishes it. This is what makes SWC1_gain change only when
 * eventG fires, decoupled from the periodic eventClockA reactivation.
 */
void SWC1_SaveGain(void)
{
    Read_gSWC1_gain(&SWC1_gain);
    Write_gSWC1_gain(SWC1_gain);
}

/*
 * SWC1_ResetGain  (mapped to eventR)
 * --------------------------------------
 * Resets SWC1's internal gain to 1, and republishes it on the global
 * interface using the same write function (i.e. gSWC1_gain is reset "by
 * using local SWC1_gain and the write function", not by writing 1 twice).
 */
void SWC1_ResetGain(void)
{
    SWC1_gain = (int32)1;
    Write_gSWC1_gain(SWC1_gain);
}

/* 
 * ---------------------------------------------------------------------
 * Additional functions to support state management for solver-rollback. 
 * ---------------------------------------------------------------------
 */

void GetState_SWC1_value(int32 *value)
{
    *value = SWC1_value;
}

void SetState_SWC1_value(const int32 value)
{
    SWC1_value = value;
}

void GetState_SWC1_gain(int32 *gain)
{
    *gain = SWC1_gain;
}

void SetState_SWC1_gain(const int32 gain)
{
    SWC1_gain = gain;
}

void GetState_SWC1_valueGained(int32 *valueGained)
{
    *valueGained = SWC1_valueGained;
}

void SetState_SWC1_valueGained(const int32 valueGained)
{
    SWC1_valueGained = valueGained;
}