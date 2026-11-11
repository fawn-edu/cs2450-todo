# Compiling

You will need the [CMake](https://cmake.org/download) build system, at least v3.30 or newer. Either install it from the website or from your preferred package manager.

You will also need the [FLTK](https://www.fltk.org/index.php) library. If you cannot install it from your preferred package manager (read: you are using Windows), perform the following actions in an elevated shell:

## Installing FLTK

```sh
git clone git@github.com:fltk/fltk.git
cd fltk
cmake -Bbuild -DFLTK_BUILD_TEST=OFF
cmake --build build --config Release --parallel 4
cmake --install build
cd ..
rm -r fltk
```

You can use `--parallel N` instead, where N is the number of logical processor cores on your system. N=4 is an educated guess.

# Building the todo application

Once these dependencies are installed, you can build the program:

```sh
# In the repository's root, typically `cs2450-taskmanager`,
cd src
cmake -Bbuild
cmake --build build
```

The resulting binary should be in `build/`. Exactly where depends on your platform; for Windows, it's usually at `build/Debug/todo.exe`, and for other platforms it's usually at `build/todo`.
