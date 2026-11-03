#ifndef SCHEDULER_EVENTS_H
#define SCHEDULER_EVENTS_H

/*
 * scheduler_events.h
 * -------------------
 * Mng_event* functions and the EventMaskType bit layout they respond to.
 *
 * NOTE: in a real AUTOSAR Classic BSW these "Mng_event*" functions would
 * not exist as such - runnables are activated directly by the OS/RTE
 * (OS alarms/schedule tables for time-triggered events such as
 * eventClockA; Rte_Ack/OS events for data/mode/event-driven ones)
 * according to the ECU configuration. There is no application-level
 * function that "manages" an event in a real stack. They are introduced
 * here purely as an explicit, simple stand-in for that low-level
 * architecture, so this whole example can be driven by a single doStep()
 * call from Modelica.
 */

#include "datatypes.h"

/*
 * events bitmask layout. MUST match the constants of the same name/value
 * (EVENT_CLOCKA_BIT / EVENT_G_BIT / EVENT_R_BIT) defined on the Modelica
 * side (see sys_appsw, in pkg_appsw.mo).
 */
#define EVENT_CLOCKA_BIT ((EventMaskType)0x01) /* time-based  (periodic clock)   */
#define EVENT_G_BIT      ((EventMaskType)0x02) /* event-based (save gain)        */
#define EVENT_R_BIT      ((EventMaskType)0x04) /* event-based (reset)            */

/*
 * Priority (highest to lowest): eventR > eventG > eventClockA.
 * doStep() (appsw.c) calls these three functions in that priority order,
 * so that - should several events coincide at the same instant - the
 * runnables of the higher-priority event are pushed onto the runnable
 * queue (and therefore executed) first. See scheduler_manager.c.
 */
void Mng_eventClockA(void);
void Mng_eventG(void);
void Mng_eventR(void);

#endif /* SCHEDULER_EVENTS_H */
