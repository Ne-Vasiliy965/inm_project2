#include "linear_system_solver.h"


//  --------------Matrix Implementation----------------
template <class T>
Matrix<T>::Matrix() noexcept : num_lines_(0), num_columns_(0) {};

template <class T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& data) {
  data_ = data;
  if (!data_.empty()) {
    num_rows_ = data_.size();
    num_columns_ = data_[0].size();
  } else {
    num_rows_ = 0;
    num_columns_ = 0;
  }
}

template <class T>
Matrix<T>::Matrix(std::vector<std::vector<T>>&& data) {
  if (!data.empty()) {
    data_ = std::move(data);
    num_rows_ = data_.size();
    num_columns_ = data_[0].size();

  } else {
    num_rows_ = 0;
    num_columns_ = 0;
  }
}

template <class T>
Matrix<T>::~Matrix() noexcept {}

template <class T>
bool Matrix<T>::check_size_is_square() noexcept {
  return num_columns_ == num_lines_;
}

template <class T>
size_t Matrix<T>::get_columns_number() {
  return num_columns_;
}

template <class T>
size_t Matrix<T>::get_rows_number() {
  return num_rows_;
}

template <class T>
std::vector<T>& Matrix<T>::operator[](size_t idx) {
  return data_[idx]
}

template <class T>
const std::vector<T>& Matrix<T>::operator[](size_t idx)  const {
  return data_[idx]
}


//  --------------Vector Implementation----------------

template <class T>
size_t Vector<T>::get_size() const {
  return data_.size();
}

template <class T>
T& Vector<T>::operator[](size_t idx) {
  return data_[idx];
}

template <class T>
const T& Vector<T>::operator[](size_t idx) const {
  return data_[idx];
}

template <class T>
Vector<T> operator+(const Vector<T>& lhs, const Vector<T>& rhs) {
  size_t cur_left_size = lhs.get_size();
  size_t cur_right_size = rhs.get_size();
  if (cur_left_size != cur_right_size) {
    throw std::logic_error("Cannot sum vectors with different sizes");
  }
  std::vector tmp(cur_left_size);
  for (size_t i = 0; i < cur_left_size; ++i) {
    tmp[i] = lhs[i] + rhs[i];
  }
  return Vector(tmp);
}

template <class T>
Vector<T> operator-(const Vector<T>& lhs, const Vector<T>& rhs) {
  size_t cur_left_size = lhs.get_size();
  size_t cur_right_size = rhs.get_size();
  if (cur_left_size != cur_right_size) {
    throw std::logic_error("Cannot substract vectors with different sizes");
  }
  std::vector tmp(cur_left_size);
  for (size_t i = 0; i < cur_left_size; ++i) {
    tmp[i] = lhs[i] - rhs[i];
  }
  return Vector(tmp);
}

template <class T>
Vector<T> operator*(const T& multiplier, const Vector<T>& vec) {
  size_t cur_size = lhs.get_size();
  std::vector tmp(cur_size);
  for (size_t i = 0; i < cur_size; ++i) {
    tmp[i] = multiplier * vec[i];
  }
  return Vector(tmp);
}

template <class T>
Vector<T> mat_vec_mul(const Matrix<T>& mat, const Vector<T>& vec) {
  size_t num_columns = mat.get_columns_number();
  size_t num_rows = mat.get_rows_number();
  if (num_columns != vec.get_size()) {
    throw std::logic_error("Cannot execute matvec multiplication: the sizes are inconsistent");
  }
  std::vector tmp(num_rows);
  for (size_t i = 0; i < num_rows; ++i) {
    T cur_elem = 0;
    for (size_t j = 0; j < num_columns; ++j) {
      cur_elem += mat[i][j] * vec[j];
    }
    tmp[i] = cur_elem;
  }
  return Vector(tmp)
}

//  --------------Solver Implementation----------------
