/*
 * swc2_runnables.c
 * ----------------
 * Software Component SWC2: internal variable (this SW-C's "Per-Instance
 * Memory") and runnables.
 */

#include "swc2_runnables.h"
#include "interfaces.h"
#include "datatypes.h"

/* SWC2 internal variable. */
static uint8 SWC2_counter = 0U;

/*
 * SWC2_Count  (mapped to eventClockA, periodic)
 * -----------------------------------------------
 * Reads the current counter, increments it (wrapping at 255 -> 0, as
 * expected of a uint8), and writes it back.
 */
void SWC2_Count(void)
{
    Read_gSWC2_counter(&SWC2_counter);
    SWC2_counter = (uint8)(SWC2_counter + 1U);
    Write_gSWC2_counter(SWC2_counter);
}

/*
 * SWC2_ResetCounter  (mapped to eventR)
 * -----------------------------------------
 * Resets the counter to 0.
 */
void SWC2_ResetCounter(void)
{
    SWC2_counter = (uint8)0U;
    Write_gSWC2_counter(SWC2_counter);
}

/* 
 * ---------------------------------------------------------------------
 * Additional functions to support state management for solver-rollback. 
 * ---------------------------------------------------------------------
 */

 /* State access to the Per-Instance Memory - see swc2_runnables.h. */
void GetState_SWC2_counter(uint8 *counter)
{
    *counter = SWC2_counter;
}

void SetState_SWC2_counter(const uint8 counter)
{
    SWC2_counter = counter;
}
