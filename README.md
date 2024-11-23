# C++ Project Template

This provides a cmake template for a C++ project of medium complexity

## Structure

```
■───┐
    ├──── cmake          cmake utilities and packages
    │
    ├──── docs           placeholder for project documentation
    │
    ├──── include        executable include files
    │
    └──── src            executable source files
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
