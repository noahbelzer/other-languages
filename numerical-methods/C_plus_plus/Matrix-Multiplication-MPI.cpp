/*
===== First Time =====
Install OpenMPI in terminal.
    In terminal, run...
        sudo apt update
        sudo apt install openmpi-bin libopenmpi-dev

    This gives you mpicc, mpic++, mpirun, and mpi.h

Remove red line under #include <mpi.h>.
    In VS Code -> Ctrl + Shift + P
    C/C++: Edit Configurations (UI)

    Set:
        Compiler path: /usr/bin/mpic++
=======================

Compile:
    mpic++ -o Matrix-Multiplication-MPI Matrix-Multiplication-MPI.cpp
    mpirun --oversubscribe -np 4 ./Matrix-Multiplication-MPI
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <mpi.h>
using namespace std;

