#ifndef SIMPLE_LINEAR_SOLVER
#define SIMPLE_LINEAR_SOLVER

#include <iostream>
#include <vector>
#include <exception>

template <class T>
class Matrix {

 private:
  size_t num_rows_;
  size_t num_columns_;
  std::vector<std::vector<T>> data_;

 public:

  Matrix() noexcept;
  Matrix(const std::vector<std::vector<T>>& data);
  Matrix(std::vector<std::vector<T>>&& data);  //  for r-value
  ~Matrix() noexcept;
  //  Copy and move constructors to be done ?
  bool check_size_is_square() noexcept;
  size_t get_columns_number();
  size_t get_rows_number();

  //  double check this
  std::vector<T>& operator[](size_t idx);
  const std::vector<T>& operator[](size_t idx) const;

  /* can add multiplying matrix by a number and operator + for matrices,
  but it is not needed in current task */
};



template <class T>
class Vector {
 private:
  std::vector<T> data_;

 public:
  Vector() noexcept default;
  Vector(const std::vector<T>& data) default;
  Vector(std::vector<T>&& dat) default;
  ~Vector() default;
  size_t get_size() const;
  T& operator[](size_t idx);
  const T& operator[](size_t idx) const;
  //  operator=??
  friend Vector operator+(const Vector& lhs, const Vector& rhs);
  friend Vector operator-(const Vector& lhs, const Vector& rhs);
  friend Vector operator*(const T& multiplier, const Vector& vec);
};


#endif