# Compiling

You will need the [CMake](https://cmake.org/download) build system, at least v3.30 or newer. [Ninja](https://ninja-build.org) is recommended as it is faster than the default options, but it is not required. Install these from their websites or from your operating system's package manager.

On Debian-based Linux systems, you will also need to install the `libgl-dev` package.
<!-- TODO: Arch-based Linux systems (libglvnd)? Others? -->

```sh
# In the repository root, presumably cs2450-todo/
# If you do not have Ninja installed, omit "-G Ninja"
cmake -S src -B src/build -DCMAKE_BUILD_TYPE=RelWithDebugInfo -G Ninja
cmake --build src/build --parallel 4
```

The resulting binary will be at `src/build/todo` or `src\build\todo.exe`.
