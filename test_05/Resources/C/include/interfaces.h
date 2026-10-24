#ifndef INTERFACES_H
#define INTERFACES_H

/*
 * interfaces.h
 * ------------
 * Global interface ("VFB"/Sender-Receiver) variables shared between the
 * Software Components (SWC1, SWC2), heavily simplified from real AUTOSAR
 * Classic.
 *
 * In full AUTOSAR Classic a Software Component never touches a global
 * variable directly: every port access goes through RTE-generated API
 * calls, e.g. Rte_Read_<port>_<data element>() / Rte_Write_<port>_<data
 * element>() (or Rte_IRead/Rte_IWrite for implicit/inter-runnable-variable
 * access), which hide where/how the data is actually stored (per-core,
 * queued, OS-resource protected, ...).
 *
 * This example keeps a single global variable per interface signal, and
 * wraps each one with a Read_* / Write_* function that plays only the ROLE
 * of that missing Rte_Read/Rte_Write layer (names simplified for this
 * exercise - a real AUTOSAR project would never call them this directly).
 * This is why swc1_runnables.c / swc2_runnables.c never reference a global
 * variable directly, and instead always go through these functions.
 */

#include "datatypes.h"

extern int32 gSWC1_value;
extern int32 gSWC1_gain;
extern int32 gSWC1_valueGained;
extern uint8 gSWC2_counter;

inline void Write_gSWC1_value(const int32 value)
{
    gSWC1_value = value;
}

inline void Write_gSWC1_gain(const int32 value)
{
    gSWC1_gain = value;
}

inline void Write_gSWC1_valueGained(const int32 value)
{
    gSWC1_valueGained = value;
}

inline void Write_gSWC2_counter(const uint8 value)
{
    gSWC2_counter = value;
}

inline void Read_gSWC1_value(int32 *value)
{
    *value = gSWC1_value;
}

inline void Read_gSWC1_gain(int32 *value)
{
    *value = gSWC1_gain;
}

inline void Read_gSWC1_valueGained(int32 *value)
{
    *value = gSWC1_valueGained;
}

inline void Read_gSWC2_counter(uint8 *value)
{
    *value = gSWC2_counter;
}


#endif /* INTERFACES_H */
