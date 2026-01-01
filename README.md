## Muon

Muon is an LLM inference implementation written in C++. 

## Build Instructions

To build, Muon requires a C++23 compatible compiler and CMake 3.31.

To build the CPU backend, use:
```
cmake --preset release
cmake --build --preset release
```

And for the SYCL backend:

```
cmake --preset sycl-release
cmake --build --preset sycl-release
```
