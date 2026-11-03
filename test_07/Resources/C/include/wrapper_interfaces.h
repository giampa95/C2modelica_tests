#ifndef WRAPPER_INTERFACES_H
#define WRAPPER_INTERFACES_H

/*
 * wrapper_interfaces.h
 * -------------------
 * Wrapper interfaces: one accessor per signal, named
 * after the interface global it accesses. The functions are
 * defined directly in this header, which is included by appsw.c only
 * (single translation unit), so no new .c file and no build changes.
 *
 * For communication with wrapper: prepareStep(simTime) [appsw.h], then the
 * SetWrapper_* inputs (after the rewind, so fresh values win over the
 * rewound state), then doStep(simTime, events) [appsw.h], then the
 * GetWrapper_* outputs, then commitStep() [appsw.h]. The interface
 * globals are part of the state manager's snapshots, so the outputs read
 * here are always consistent with the (possibly rewound) C state.
 */

#include "datatypes.h"  /* int32 */
#include "interfaces.h" /* gSWC1_value, gSWC1_gain, gSWC1_valueGained, gSWC2_counter */

/* "Required" input: the value read by SWC1. */
void SetWrapper_gSWC1_value(const int value)
{
    gSWC1_value = (int32)value;
}

/* "Required" input: the gain read by SWC1. */
void SetWrapper_gSWC1_gain(const int gain)
{
    gSWC1_gain = (int32)gain;
}

/* "Provided" output: SWC1_value * SWC1_gain. */
void GetWrapper_gSWC1_valueGained(int *valueGained)
{
    *valueGained = (int)gSWC1_valueGained;
}

/* "Provided-required" output: SWC2's free-running counter. */
void GetWrapper_gSWC2_counter(int *counter)
{
    *counter = (int)gSWC2_counter;
}

#endif /* WRAPPER_INTERFACES_H */