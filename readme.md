# Multi-threaded Algorithms in C

This repository contains two multi-threaded algorithms implemented in C:

1. Multi-threaded Merge Sort
2. Multi-threaded Matrix Multiplication

## Multi-threaded Merge Sort

### Introduction

This program demonstrates the implementation of multi-threaded merge sort in C, utilizing the pthread library.

### Features

- Multi-threaded merge sort for improved performance.
- Dynamic memory allocation for variable-sized arrays.
- Reading input data from a file for sorting.

### Usage

To use this program:

1. Compile the source code:

    ```bash
    make
    ```

2. Run the executable with the input file:

    ```bash
    ./7609_sort input.txt
    ```

    Replace `input.txt` with the path to your input file.


## Multi-threaded Matrix Multiplication

### Introduction

This program performs multi-threaded matrix multiplication in C, utilizing the pthread library. It reads input matrices from a file, performs multiplication using both element-wise and row-wise approaches, and prints the results along with execution times.

### Features

- Multi-threaded matrix multiplication for improved performance.
- Reading input matrices from a file.
- Outputting results for both element-wise and row-wise multiplication.

### Usage

To use this program:

1. Compile the source code:

    ```bash
    make
    ```

2. Run the executable with the input file:

    ```bash
    ./7609-matrix input.txt
    ```

    Replace `input.txt` with the path to your input file.

