# Simple tests for C code integration in OpenModelica

List of tests:

- Test 01: Integration of the "pow" function from standard libc cmath library.
- Test 02: Integration of a custom C implementation of the ReLu function returning the result by value.
- Test 03: Integration of a custom C implementation of the ReLu function returning the result by pointer.
- Test 04: Integration of a pre-compiled x86-64 static library (.lib), obtained by a C implementation of the ReLu function returning the result by value.
- Test 05: Integration of a pre-compiled x86-64 dynamic library (.dll), obtained by a C implmentation of an AUTOSAR-Classic-inspired application software.
- Test 06: Integration of a pre-compiled x86-64 dynamic library (.dll), obtained by a C implmentation of an AUTOSAR-Classic-inspired application software. Solver's rollback is also handled.

Plan of tests to be done:

- Test 06 need testing. The variable 'time' is not allowed in a function.
- Other tests are not planned.
