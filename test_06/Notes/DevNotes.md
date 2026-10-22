This document outlines key technical constraints and observed behaviors regarding the Modelica C-library wrapper (pkg_appsw.mo).

1. Discrete Variable Limitations in Modelica Standard Library (MSL)
- Limited MSL Support: While input signals to the Modelica block should ideally be declared using the discrete variability prefix, the Modelica Standard Library (MSL) offers limited standard support for this pattern in signal blocks.
- In the current version, representation of variables is handled through "integer limits" in Modelica and "casting" in C. 

2. Simultaneous Event Handling & Task Scheduling
- Execution Desynchronization: In current implementation, concurrent events occurring at the exact same simulation time step are currently not processed simultaneously on the Modelica side. Instead, runnable groups associated with these concurrent events are scheduled across distinct time instances.
- Root Cause: This desynchronization appears to be a direct consequence of avoiding discrete variable declarations to work around MSL/compiler constraints.

3. Recommendations for Future Developer Work
- Consider using explicit boolean event triggers (e.g., via edge() or sample()) to force strict atomic event evaluation.
- Ensure the event bitmask/array (e.g., [eventR, eventG, eventClockA]) is fully resolved within a single sampling instant before invoking the C doStep entry point.
