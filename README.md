# MatricesCalculator# Matrix Calculator

This repository contains a simple matrix calculator implemented in C. The calculator supports basic matrix operations such as addition, subtraction, multiplication, scalar multiplication, and transposition.

## Files

### `mainmat.c`

This file contains the main functionality of the matrix calculator. It includes the main function, which handles user interaction, command analysis, and matrix operations. The supported commands include `read_mat`, `print_mat`, `add_mat`, `sub_mat`, `mul_mat`, `mul_scalar`, `trans_mat`, and `stop`.

### `mymat.c`

This file includes the arithmetic functions for matrices. It defines functions for reading, printing, adding, subtracting, multiplying, multiplying by a scalar, and transposing matrices.

### `mymat.h`

This header file contains the typedef for the `mat` structure and declarations for all the functions defined in `mymat.c`.

### `makefile`

The makefile is provided to compile the program. It specifies the dependencies and compilation instructions.

## Usage

To use the matrix calculator, compile the program using the provided makefile:

```bash
make mainmat