/*
 * appsw.c
 * -------
 * Main entry points of the App SW dynamic library: constructor,
 * destructor, doStep. See appsw.h for the full contract of each.
 *
 * test_07 (FMI-like interfaces): the SetWrapper_* / GetWrapper_* accessors
 * are defined directly in wrapper_interfaces.h; prepareStep / commitStep
 * below replace the StateMng_prepareStep() / StateMng_commitStep() calls
 * that test_06 made inside doStep().
 */

#include "appsw.h"
#include "datatypes.h"
#include "interfaces.h"
#include "scheduler_events.h"
#include "scheduler_manager.h"
#include "state_manager.h"      /* Handling solver's rollback */
#include "wrapper_interfaces.h" /* Wrapper interfaces */

void prepareStep(double simTime)
{
    /* Handling solver's rollback - driven by the environment before
     * setting the inputs (see appsw.h / wrapper_interfaces.h). */
    StateMng_prepareStep(simTime);
}

void commitStep(void)
{
    /* Handling solver's rollback - driven by the environment at the end
     * of the communication point. */
    StateMng_commitStep();
}

void constructor(void)
{
    resetScheduler();

    /* Forced reset as in test_06 (eventR at t=0, zero inputs), through
     * the FMI-like sequence - see wrapper_interfaces.h. */
    prepareStep((double)0.0);
    SetWrapper_gSWC1_value((int)0);
    SetWrapper_gSWC1_gain((int)0);
    doStep((double)0.0, EVENT_R_BIT);
    commitStep();

    /* Solver's rollback handling */
    StateMng_init();
}

void destructor(void)
{
    resetScheduler();

    /* Solver's rollback handling */
    StateMng_reset();
}

void doStep(double simTime, EventMaskType events)
{
    
    /* simTime is now a placeholder, not really needed*/
    
    /* Wrapper interfaces are now implemented via wrapper_interfaces.h. 
     * prepareStep() / commitStep() are now directly called via the wrapper.
       doStep() is not used anymore to set internal interfaces, 
       SetWrapper_* / GetWrapper_* are used instead. */

    /* StateMng_prepareStep(simTime); */

    /* gSWC1_value = (int32)R_SWC1_value; */
    /* gSWC1_gain  = (int32)R_SWC1_gain;  */

    /* Highest priority first: eventR > eventG > eventClockA. Runnables run
     * in the order they are pushed (see scheduler_manager.c), so calling
     * Mng_event* in priority order guarantees the correct execution order
     * even when several events fire at the very same instant. */
    if ((events & EVENT_R_BIT) != 0)
    {
        Mng_eventR();
    }
    if ((events & EVENT_G_BIT) != 0)
    {
        Mng_eventG();
    }
    if ((events & EVENT_CLOCKA_BIT) != 0)
    {
        Mng_eventClockA();
    }

    runRunnables();

    /* *P_SWC1_valueGained = (int)gSWC1_valueGained; */
    /* *PR_SWC2_counter    = (int)gSWC2_counter;    */

    /* StateMng_commitStep(); */

}