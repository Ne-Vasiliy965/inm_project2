#ifndef SIMPLE_LINEAR_SOLVER
#define SIMPLE_LINEAR_SOLVER

#include <iostream>
#include <vector>

template <class T>

class Matrix {

 private:
  size_t num_lines_;
  size_t num_columns_;
  std::vector<std::vector<T>> data_;

 public:

  Matrix() noexcept;
  Matrix(std::vector<sdtd::vector<T>> data);
  ~Matrix() noexcept;
};



#endif