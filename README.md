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

```bash
cmake -B build -S .
cd build
make
```

## Unit Tests

Tests are built automatically and can be run from the **build** directory by:

```bash
tests/unit_tests
```

You can prevent the tests being built by passing in the cmake parameter **-DBUILD_TESTS=OFF**:

```bash
cmake -B build -S . -DBUILD_TESTS=OFF
cd build
make
```
