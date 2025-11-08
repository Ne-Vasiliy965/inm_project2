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
  bool is_square() const noexcept;
  bool is_symmetrical() const noexcept;
  size_t get_columns_number();
  size_t get_rows_number();

  //  double check this
  std::vector<T>& operator[](size_t idx);
  const std::vector<T>& operator[](size_t idx) const;

  /* can add multiplying matrix by a number and operator + for matrices,
  but it is not needed in current task */

  friend Vector mat_vec_mul(const Matrix& matrix, const Vector& vec);
};



template <class T>
class Vector {
 private:
  std::vector<T> data_;

 public:
  Vector() noexcept = default;
  Vector(const std::vector<T>& data) = default;
  Vector(std::vector<T>&& dat) = default;
  ~Vector() default;
  size_t get_size() const;
  T& operator[](size_t idx);
  const T& operator[](size_t idx) const;
  //  operator=??
  friend Vector operator+(const Vector& lhs, const Vector& rhs);
  friend Vector operator-(const Vector& lhs, const Vector& rhs);
  friend Vector operator*(const T& multiplier, const Vector& vec);
  friend Vector mat_vec_mul(const Matrix& mat, const Vector& vec);
};

  /*Solver for only symmetric positively defined matrix.
    In this version you need to give the estimation of 
    max and min eigenvalues of matrix.
    Currently only the type double for matrix*/
class SimpleLinearSolver {
 private:
  double epsilon;
  bool stop_by_max_iter;
  size_t max_iter;
  Matrix<double> mat;
  Vector<double> rhs;
  double min_eigenval;
  double max_eigenval;
  Vector<double> cur_ans;
  Vector<double> simple_iteration(Vector<double> cur);
  Vector<double> simple_iteration_method();

 public:
  SimpleLinearSolver(double epsilon, Matrix<double> mat, Vector<double> rhs,
                     double min_ei, double max_ei, bool stop_by_max_iter = false, 
                     size_t max_iter = 0);

  SimpleLinearSolver(double epsilon, Matrix<double> mat, Vector<double> rhs,
                     Vector<double> starting_point, double min_ei, double max_ei,
                     bool stop_by_max_iter = false, size_t max_iter = 0);

  ~SimpleLinearSolver() = default;

  Vector<double> solve();
};
#endif