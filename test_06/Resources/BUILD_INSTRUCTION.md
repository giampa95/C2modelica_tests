
## Building with `make` in Windows

```sh
cd Resources/C

make            # builds for the host platform (auto-detected)
make windows    # force a Windows build  -> ../Build/appsw.dll
make clean      # remove all build output
```

Object files are platform-specific: run `make clean` before switching from
`make linux` to `make windows` or vice versa.

## Building manually with `gcc` in Windows

```sh

cd Resources/C
mkdir -p ../Build

# Always run 'make clean' (or delete ../Build/*.o and appsw.dll) first, and after ANY change to
# src/*.c or include/*.h, rebuild the DLL -- Modelica will keep loading the stale one otherwise.

gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/datatypes.c        -o ../Build/datatypes.o
gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/interfaces.c       -o ../Build/interfaces.o
gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/scheduler_manager.c -o ../Build/scheduler_manager.o
gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/scheduler_events.c -o ../Build/scheduler_events.o
gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/swc1_runnables.c   -o ../Build/swc1_runnables.o
gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/swc2_runnables.c   -o ../Build/swc2_runnables.o
gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/appsw.c            -o ../Build/appsw.o
gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/state_interfaces.c -o ../Build/state_interfaces.o
gcc -std=c99 -Wall -Wextra -O2 -fPIC -Iinclude -c src/state_manager.c   -o ../Build/state_manager.o
gcc -shared -o ../Build/appsw.dll ../Build/*.o

```
