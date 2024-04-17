# OpenCL Vector Addition and Device Info

This project contains two programs written in C that demonstrate the use of OpenCL:

1. `vector_add`: A program that performs vector addition using OpenCL. It adds two arrays of floating-point numbers element-wise.
2. `device_info`: A program that displays information about the OpenCL platforms and devices available on your system.

## Prerequisites

To compile and run these programs, you need:

- OpenCL development libraries installed on your system.
- OpenCL drivers for your GPU installed and functioning correctly.

## Compilation

To compile the programs, use the provided Makefile:

```bash
make
./vector_add
./device_info

