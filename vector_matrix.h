#ifndef VECTOR_MATRIX
#define VECTOR_MATRIX

#include <vector>
#include <stdexcept>
#include <cmath>
#include <iostream>

template <class T>
class Vector;

template <class T>
class Matrix {

 private:
  size_t num_rows_;
  size_t num_columns_;
  std::vector<std::vector<T>> data_;

 public:

  Matrix() noexcept : num_rows_(0), num_columns_(0) {};
  Matrix(const std::vector<std::vector<T>>& data) {
    data_ = data;
    if (!data_.empty()) {
      num_rows_ = data_.size();
      num_columns_ = data_[0].size();
    } else {
      num_rows_ = 0;
      num_columns_ = 0;
    }
  }
  Matrix(std::vector<std::vector<T>>&& data) {
    if (!data.empty()) {
      data_ = std::move(data);
      num_rows_ = data_.size();
      num_columns_ = data_[0].size();
    } else {
      num_rows_ = 0;
      num_columns_ = 0;
    }
  }
  ~Matrix() noexcept = default;

  bool is_square() const noexcept {
    return num_columns_ == num_rows_;
  }


  bool is_symmetrical() const noexcept {
    if (!is_square()) {
      return false;
    }
    bool ans = true;
    for (size_t i = 0; i < num_rows_ - 1; ++i) {
      for (size_t j = i + 1; j < num_columns_; ++j) {
        if (data_[i][j] != data_[j][i]) {
          ans = false;
          break;
        }
      }
      if (!ans) {
        break;
      }
    }
    return ans;
  }

  size_t get_columns_number() const {
    return num_columns_;
  }

  size_t get_rows_number() const {
    return num_rows_;
  }

  //  double check this
  std::vector<T>& operator[](size_t idx) {
    return data_[idx];
  }
  const std::vector<T>& operator[](size_t idx)  const {
    return data_[idx];
  }

  /* can add multiplying matrix by a number and operator + for matrices,
  but it is not needed in current task */
};

template <class T>
class Vector {
 private:
  std::vector<T> data_;

 public:
  Vector() noexcept = default;
  Vector(const std::vector<T>& data) : data_(data) {}
  Vector(std::vector<T>&& data) : data_(std::move(data)) {}
  ~Vector() = default;

  size_t get_size() const {
    return data_.size();
  }
  //  norm type - double
  double get_euclidean_norm() const {
    double squared_norm = 0;
    size_t size = get_size();
    for (size_t i = 0; i < size; ++i) {
      squared_norm += std::pow(static_cast<double>(data_[i]), 2);
    }
    return std::sqrt(squared_norm);
  }

  T& operator[](size_t idx) {
    return data_[idx];
  }
  const T& operator[](size_t idx) const {
    return data_[idx];
  }
  //  operator=??
  Vector<T>& operator+= (const Vector& other) {
    size_t size = this->get_size();
    size_t rhs_size = other.get_size();
    if (size != rhs_size) {
      throw std::logic_error("Cannot sum vectros with different sizes");
    }
    for (size_t i = 0; i < size; ++i) {
      this->data_[i] += other[i];
    }
    return *this;
  }

  // change the operators implementation with +=
  friend Vector operator+(const Vector<T>& lhs, const Vector<T>& rhs) {
    size_t cur_left_size = lhs.get_size();
    size_t cur_right_size = rhs.get_size();
    if (cur_left_size != cur_right_size) {
      throw std::logic_error("Cannot sum vectors with different sizes");
    }
    std::vector<T> tmp(cur_left_size);
    for (size_t i = 0; i < cur_left_size; ++i) {
      tmp[i] = lhs[i] + rhs[i];
    }
    return Vector(tmp);
  }
  

  friend Vector operator-(const Vector<T>& lhs, const Vector<T>& rhs) {
    size_t cur_left_size = lhs.get_size();
    size_t cur_right_size = rhs.get_size();
    if (cur_left_size != cur_right_size) {
      throw std::logic_error("Cannot substract vectors with different sizes");
    }
    std::vector<T> tmp(cur_left_size);
    for (size_t i = 0; i < cur_left_size; ++i) {
      tmp[i] = lhs[i] - rhs[i];
    }
    return Vector(tmp);
  }


  friend Vector operator*(const T& multiplier, const Vector<T>& vec) {
    size_t cur_size = vec.get_size();
    std::vector<T> tmp(cur_size);
    for (size_t i = 0; i < cur_size; ++i) {
      tmp[i] = multiplier * vec[i];
    }
    return Vector(tmp);
  }
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
  size_t s = vec.get_size();
  for (size_t i = 0; i < s; ++i) {
    os << vec[i] << ' ';
  }
  return os;
}

template <class T>
Vector<T> mat_vec_mul(const Matrix<T>& mat, const Vector<T>& vec) {
  size_t num_columns = mat.get_columns_number();
  size_t num_rows = mat.get_rows_number();
  if (num_columns != vec.get_size()) {
    throw std::logic_error("Cannot execute matvec multiplication: the sizes are inconsistent");
  }
  std::vector<T> tmp(num_rows);
  for (size_t i = 0; i < num_rows; ++i) {
    T cur_elem = 0;
    for (size_t j = 0; j < num_columns; ++j) {
      cur_elem += mat[i][j] * vec[j];
    }
    tmp[i] = cur_elem;
  }
  return Vector(tmp);
}

#endif