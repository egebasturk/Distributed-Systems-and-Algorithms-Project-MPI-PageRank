#include <cstdio>
#include <iostream>
#include <malloc.h>
#include <mpi.h>
#include <cmath>


int readMatrixFromFile(int** arr, const char* input_filename, int *N);
int readVectorFromFile(int** arr, const char* input_filename, int *N);