# Simple tests for C code integration in OpenModelica

## List of tests

- **Test 01**: Integration of the "pow" function from standard libc cmath library.
- **Test 02**: Integration of a custom C implementation of the ReLu function returning the result by value.
- **Test 03**: Integration of a custom C implementation of the ReLu function returning the result by pointer.
- **Test 04**: Integration of a pre-compiled x86-64 static library (.lib), obtained by a C implementation of the ReLu function returning the result by value.
- **Test 05**: Integration of a pre-compiled x86-64 dynamic library (.dll), obtained by a C implmentation of an AUTOSAR-Classic-inspired application software.
- **Test 06**: Integration of a pre-compiled x86-64 dynamic library (.dll), obtained by a C implmentation of an AUTOSAR-Classic-inspired application software. Solver's rollback is also handled.
- **Test 07**: Integration of a pre-compiled x86-64 dynamic library (.dll), obtained by a C implmentation of an AUTOSAR-Classic-inspired application software. Solver's rollback is also handled. Differently from *Test 06*, specific functions are used to set and get values from Modelica wrapper to C code. 

## List of planned tests and updates

- **Test 06** and **Test 07** need testing to verify the correct behaviour in solver's rollback handling.
- For **Test 05**, **Test 06** and **Test 07** need to investigate if different names would be more appropriate for the inputs and outputs of the Application Software. Currently using names like `R_<name>`, `P_<name>` and `PR_<name>`.
- Further tests are not currently planned.
