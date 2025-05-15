# C++ Project Template

This provides a cmake template for a C++ project of medium complexity

## Structure

The template project structure is for a library (both static and shared) that holds all the project code.
An executable which can run the library code at the command line.
And unit tests which exercises the library code. The layout of this project structure is shown below:

```
■───┐
    ├──── cmake                                  cmake utilities and packages
    │
    ├──── docs                                   placeholder for project documentation
    │
    ├──── include                                executable include files
    │
    ├──── src ───┐                               executable source files
    │            │
    │            └──── lib ───┐                  library for the project that the executable links to
    │                         │
    │                         ├──── include      library include files
    │                         │
    │                         └──── src          library source files
    │
    └──── tests ─┐                               project unit tests
                 │
                 ├──── common                    common test utility source files
                 │
                 └──── include                   common test utility include files
```

## Prerequisites

This template has been tested with the following toolchain:

-   cmake version 3.22.1
-   gcc version 11.4.0

## Build Instructions

Build is done via presets, you can list them by:

```
cmake --list-presets
```

For a clean release build

```bash
rm -rf build && cmake --preset=release && cmake --build build -j
```

## Unit Tests

To build and run the units tests use the test default

```bash
rm -rf build && cmake --preset=default-with-tests && cmake --build build -j
cd build && ./unit_tests
```

You can build the tests for release by overriding the preset settings:

```
rm -rf build && cmake --preset=default-with-tests -CMAKE_BUILD_TYPE=Release && cmake --build build -j
```
