#!/bin/bash
set -e
if [ ! -f "liblinear_solver.a" ]; then
    echo "Compile the lib before testing with ./compile_project.sh"
    exit 1
fi

g++ test.cpp -L. -llinear_solver -o test_program
./test_program