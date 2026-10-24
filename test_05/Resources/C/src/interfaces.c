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