/*
 * datatypes.c
 * -----------
 * The type aliases declared in datatypes.h necessarily live in the header
 * itself (every translation unit using int32/uint8 must see the typedef).
 * This .c file exists for structural symmetry with the rest of the App SW
 * (datatypes / interfaces / runnables / scheduler are each split into a
 * .h/.c pair) and hosts the datatype-related definitions that DO belong in
 * a translation unit: the range constants declared as `extern` in
 * datatypes.h.
 */

#include "datatypes.h"
#include <stdint.h>

const int32 INT32_MIN_VALUE = INT32_MIN;
const int32 INT32_MAX_VALUE = INT32_MAX;
const uint8 UINT8_MIN_VALUE  = 0U;
const uint8 UINT8_MAX_VALUE  = UINT8_MAX;
