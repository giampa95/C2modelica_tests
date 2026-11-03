## Building with `make` in Windows

From 'Resources' folder, execute following 'Windows PowerShell' commands.

```sh
cd .\C

make            # builds for the host platform (auto-detected)
make windows    # force a Windows build  -> ../Build/appsw.dll
make clean      # remove all build output
```

Object files are platform-specific: run `make clean` before switching from
`make linux` to `make windows` or vice versa.

## Building manually with `gcc` in Windows

From 'Resources' folder, execute following 'Windows PowerShell' commands.

```sh

# clean up the wrongly-placed folder from the previous attempt
Remove-Item -Recurse -Force .\Build

cd .\C

if (Test-Path ..\Build) { Remove-Item -Recurse -Force ..\Build }
New-Item -ItemType Directory ..\Build | Out-Null

$cflags = "-std=c99","-Wall","-Wextra","-O2","-fPIC","-Iinclude"

gcc $cflags -c src\datatypes.c         -o ..\Build\datatypes.o
gcc $cflags -c src\interfaces.c        -o ..\Build\interfaces.o

gcc $cflags -c src\swc1_runnables.c    -o ..\Build\swc1_runnables.o
gcc $cflags -c src\swc2_runnables.c    -o ..\Build\swc2_runnables.o

gcc $cflags -c src\scheduler_manager.c -o ..\Build\scheduler_manager.o
gcc $cflags -c src\scheduler_events.c  -o ..\Build\scheduler_events.o

gcc $cflags -c src\state_manager.c     -o ..\Build\state_manager.o

gcc $cflags -c src\appsw.c             -o ..\Build\appsw.o

gcc -shared -o ..\Build\appsw.dll ..\Build\*.o

cd ..

```
