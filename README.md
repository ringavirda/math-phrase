# Math-phrase interpreter

This is a simple implementation of math expression analyzer that can understand and evaluate basic phrases. It has two main work modes: per-operation and per-phrase. The former behaves as regular calculator that can perform basic arithmetic, but the latter can interpret and collapse whole expression.

- Supported operations for both modes: { +, –. *, / }
- Per-phrase supports prentices.
- Can print out analysis information if that is necessary.
- Has fairly usable command line interface.

The main purpose of this project was to show how math can be evaluated properly and how C++ works. Also this project uses CMake as it's build system and there are build and task files for vscode to launch it. If you for some reason don't want to use normal text editors you always can build it manually.

```
> mkdir build
> cmake -S . -B build
> cmake --build build
> ./bin/math-int.exe (or ./bin/Debug/math-int.exe)
```

Just be sure to have C++ compiler of choice in your path so CMake can find it.
