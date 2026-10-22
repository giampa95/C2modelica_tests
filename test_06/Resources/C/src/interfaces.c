/*
 * interfaces.c
 * ------------
 * Storage and Read_* / Write_* accessors for the global interface variables
 * shared between SWC1 and SWC2. See interfaces.h for the design rationale.
 */

#include "interfaces.h"

int32 gSWC1_value       = 0;
int32 gSWC1_gain        = 0;
int32 gSWC1_valueGained = 0;
uint8 gSWC2_counter     = 0U;

void Write_gSWC1_value(const int32 value)
{
    gSWC1_value = value;
}

void Write_gSWC1_gain(const int32 value)
{
    gSWC1_gain = value;
}

void Write_gSWC1_valueGained(const int32 value)
{
    gSWC1_valueGained = value;
}

void Write_gSWC2_counter(const uint8 value)
{
    gSWC2_counter = value;
}

void Read_gSWC1_value(int32 *value)
{
    *value = gSWC1_value;
}

void Read_gSWC1_gain(int32 *value)
{
    *value = gSWC1_gain;
}

void Read_gSWC1_valueGained(int32 *value)
{
    *value = gSWC1_valueGained;
}

void Read_gSWC2_counter(uint8 *value)
{
    *value = gSWC2_counter;
}
