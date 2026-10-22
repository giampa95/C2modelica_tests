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

void GetState_gSWC1_value(int32 *value);
void SetState_gSWC1_value(const int32 value);

void GetState_gSWC1_gain(int32 *gain);
void SetState_gSWC1_gain(const int32 gain);

void GetState_gSWC1_valueGained(int32 *valueGained);
void SetState_gSWC1_valueGained(const int32 valueGained);

void GetState_gSWC2_counter(uint8 *counter);
void SetState_gSWC2_counter(const uint8 counter);

#endif /* STATE_INTERFACES_H */