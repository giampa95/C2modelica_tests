#ifndef APPSW_H
#define APPSW_H

/*
 * appsw.h
 * -------
 * Public interface of the App SW dynamic library, as called from Modelica
 * (see class_appsw in pkg_appsw.mo). Exactly three functions are exposed:
 * constructor, destructor and doStep.
 *
 * Because this DLL is precompiled and holds a single, global set of C
 * states (interfaces.c, swc1_runnables.c, swc2_runnables.c,
 * scheduler_manager.c), there is only ever one "instance" of the App SW:
 * no opaque per-instance handle/context is needed, and none of these
 * three functions takes or returns one.
 */

#include "datatypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * constructor
 * -----------
 * One-time initialization. Internally behaves exactly like doStep(), but
 * always forces eventR (see scheduler_events.h), guaranteeing the App SW
 * starts from a well-defined (reset) state regardless of whatever the
 * first external inputs happen to be.
 */
void constructor(void);

/*
 * destructor
 * ----------
 * One-time finalization/cleanup. This example has no dynamically
 * allocated resource to release (see note above); the function is still
 * provided so the Modelica-side lifecycle (constructor / step* / destructor)
 * stays complete and symmetrical, and to give a natural place to add
 * cleanup code should the App SW later be extended (e.g. closing a log).
 */
void destructor(void);

/*
 * doStep
 * ------
 * One reactivation of the App SW.
 *
 *   simTime             simulation time - needed to handle solver's roolback.
 *   events              bitmask of the discrete event(s) active at this
 *                       instant (see EVENT_*_BIT in scheduler_events.h).
 *                       More than one bit may be set at once.
 *   R_SWC1_value        "Required" input: value read by SWC1.
 *   R_SWC1_gain         "Required" input: gain read by SWC1.
 *   P_SWC1_valueGained  "Provided" output: SWC1_value * SWC1_gain.
 *   PR_SWC2_counter     "Provided" output: SWC2's free-running counter,
 *                       surfaced as a double (its native type is uint8)
 *                       for direct interfacing with Modelica's Real.
 */
void doStep(double simTime,
            EventMaskType events,
            int R_SWC1_value,
            int R_SWC1_gain,
            int *P_SWC1_valueGained,
            int *PR_SWC2_counter);

#ifdef __cplusplus
}
#endif

#endif /* APPSW_H */
