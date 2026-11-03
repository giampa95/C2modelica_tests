#ifndef DATATYPES_H
#define DATATYPES_H

/*
 * datatypes.h
 * -----------
 * AUTOSAR-Classic-inspired platform data types (heavily simplified).
 *
 * In a real AUTOSAR Classic stack these aliases would be generated as
 * Platform_Types.h (from the MCU/compiler abstraction), with application
 * data types layered on top in Std_Types.h / Rte_Type.h. For this example
 * we only need the two types requested, aliased directly onto the C99
 * fixed-width integer types from <stdint.h> for portability.
 */

#include <stdint.h>

typedef int32_t int32;  /* Integer of 32 bits (signed).  */
typedef uint8_t  uint8;  /* Unsigned integer of 8 bits.    */

/*
 * EventMaskType
 * -------------
 * Bitmask type used to tell the App SW which discrete event(s) -
 * eventClockA, eventG, eventR - are active on a given reactivation (see the
 * EVENT_*_BIT constants in scheduler_events.h). More than one bit can be
 * set at once: this is exactly the mechanism that lets two (or three)
 * events occurring at the very same simulation instant be handled
 * together, in a single doStep() call.
 *
 * AUTOSAR would normally use an unsigned type (e.g. uint32) for a bitmask.
 * Modelica, however, has no unsigned integer type - its "Integer" is a
 * 32-bit signed type. To keep the Modelica <-> C interface a direct,
 * ABI-safe 1:1 mapping (Modelica Integer <-> C int32_t), a signed 32-bit
 * integer is used here as well; its value is simply treated as
 * non-negative by convention.
 */
typedef int32 EventMaskType;

/* Range constants for the two platform types above (defined in
 * datatypes.c). Not used elsewhere in this example; provided for
 * completeness / for SW-Cs that may want to range-check a value. */
extern const int32 INT32_MIN_VALUE;
extern const int32 INT32_MAX_VALUE;
extern const uint8  UINT8_MIN_VALUE;
extern const uint8  UINT8_MAX_VALUE;

#endif /* DATATYPES_H */
