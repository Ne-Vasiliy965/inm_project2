#!/bin/bash
set -e
echo "Compiling project to a lib"

g++ -c linear_system_solver.cpp -o linear_system_solver.o

if [ ! -f "linear_system_solver.o" ]; then
    echo "Error occured while compiling linear_system_solver.cpp"
    exit 1
fi

ar rcs liblinear_solver.a linear_system_solver.o

if [ ! -f "liblinear_solver.a" ]; then
    echo "Error occured while archieving object file"
    exit 1
fi

echo "Static lib created successfully"