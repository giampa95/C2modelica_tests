#ifndef STATE_INTERFACES_H
#define STATE_INTERFACES_H

/*
 * state_interfaces.h
 * -------------------
 * State access to the App SW's interface globals - the state-layer
 * counterpart of interfaces.h, which stays untouched. 
 * One GetState_ / SetState_ pair per global, defined like the normal interfaces.
 */

#include "datatypes.h"

extern int32 gSWC1_value;
extern int32 gSWC1_gain;
extern int32 gSWC1_valueGained;
extern uint8 gSWC2_counter;

inline void GetState_gSWC1_value(int32 *value)
{
    *value = gSWC1_value;
}

inline void SetState_gSWC1_value(const int32 value)
{
    gSWC1_value = value;
}

inline void GetState_gSWC1_gain(int32 *gain)
{
    *gain = gSWC1_gain;
}

inline void SetState_gSWC1_gain(const int32 gain)
{
    gSWC1_gain = gain;
}

inline void GetState_gSWC1_valueGained(int32 *valueGained)
{
    *valueGained = gSWC1_valueGained;
}

inline void SetState_gSWC1_valueGained(const int32 valueGained)
{
    gSWC1_valueGained = valueGained;
}

inline void GetState_gSWC2_counter(uint8 *counter)
{
    *counter = gSWC2_counter;
}

inline void SetState_gSWC2_counter(const uint8 counter)
{
    gSWC2_counter = counter;
}

#endif /* STATE_INTERFACES_H */