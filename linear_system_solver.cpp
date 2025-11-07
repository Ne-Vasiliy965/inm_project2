#include "linear_system_solver.h"

template <class T>
Matrix::Matrix() noexcept : num_lines_(0), num_columns_(0) {};

template <class T>
Matrix::Matrix(std::vector<std::vector<T>> data) {
  data_ = data;
  if (!data_.empty()):
    num_lines_ = data_.size();
    num_columns_ = data_[0].size();
  } else {
    num_lines_ = 0;
    num_columns_ = 0;
  }

Matrix::~Matrix() noexcept {}

