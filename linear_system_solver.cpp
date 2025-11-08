#include "linear_system_solver.h"


//  --------------Matrix Implementation----------------
template <class T>
Matrix<T>::Matrix() noexcept : num_rows_(0), num_columns_(0) {};

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
bool Matrix<T>::is_square() const noexcept {
  return num_columns_ == num_rows_;
}

template <class T>
bool Matrix<T>::is_symmetrical() const noexcept {
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


template <class T>
size_t Matrix<T>::get_columns_number() const {
  return num_columns_;
}

template <class T>
size_t Matrix<T>::get_rows_number() const {
  return num_rows_;
}

template <class T>
std::vector<T>& Matrix<T>::operator[](size_t idx) {
  return data_[idx];
}

template <class T>
const std::vector<T>& Matrix<T>::operator[](size_t idx)  const {
  return data_[idx];
}


//  --------------Vector Implementation----------------

template <class T>
Vector<T>::Vector(const std::vector<T>& data) : data_(data) {}

template <class T>
Vector<T>::Vector(std::vector<T>&& data) : data_(std::move(data)) {}

template <class T>
size_t Vector<T>::get_size() const {
  return data_.size();
}

template <class T>
double Vector<T>::get_euclidean_norm() const {
  double squared_norm = 0;
  size_t size = get_size();
  for (size_t i = 0; i < size; ++i) {
    squared_norm += std::pow(static_cast<double>(data_[i]), 2);
  }
  return std::sqrt(squared_norm);
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
Vector<T>& Vector<T>::operator+= (const Vector& other) {
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
  std::vector<T> tmp(cur_left_size);
  for (size_t i = 0; i < cur_left_size; ++i) {
    tmp[i] = lhs[i] - rhs[i];
  }
  return Vector(tmp);
}

template <class T>
Vector<T> operator*(const T& multiplier, const Vector<T>& vec) {
  size_t cur_size = vec.get_size();
  std::vector<T> tmp(cur_size);
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

//  --------------Solver Implementation----------------

SimpleLinearSolver::SimpleLinearSolver(
                     double epsilon,
                     Matrix<double> mat,
                     Vector<double> rhs,
                     double min_ei,
                     double max_ei,
                     bool stop_by_max_iter,
                     size_t max_iter) :
                     epsilon(epsilon), min_eigenval(min_ei), max_eigenval(max_ei),
                     stop_by_max_iter(stop_by_max_iter), max_iter(max_iter)
{
  if (!mat.is_square()) {
    throw std::logic_error("Cannot work with non square matrix");
  }
  if (!mat.is_symmetrical()) {
    throw std::logic_error("Cannot work with non-symmetric matrix");
  }
  mat = mat;
  rhs = rhs;
  size_t dim = rhs.get_size();
  std::vector<double> tmp(dim, 0);
  cur_ans = Vector(tmp);
}

SimpleLinearSolver::SimpleLinearSolver(
                     double epsilon,
                     Matrix<double> mat,
                     Vector<double> rhs,
                     Vector<double> start_point,
                     double min_ei,
                     double max_ei,
                     bool stop_by_max_iter,
                     size_t max_iter) : 
                     epsilon(epsilon), min_eigenval(min_ei), max_eigenval(max_ei),
                     stop_by_max_iter(stop_by_max_iter), max_iter(max_iter)
{
  if (!mat.is_square()) {
    throw std::logic_error("Cannot work with non square matrix");
  }
  if (!mat.is_symmetrical()) {
    throw std::logic_error("Cannot work with non-symmetric matrix");
  }
  mat = mat;
  rhs = rhs;
  if (start_point.get_size() != mat.get_columns_number()) {
    throw std::logic_error("Invalid starting point. Check the dimension");
  }
  cur_ans= start_point;
}

void SimpleLinearSolver::simple_iteration_method() {
  size_t cur_step = 0;
  double lr = 2 / (min_eigenval + max_eigenval);
  while (true) {
    cur_step += 1;
    cur_ans += lr * (rhs - mat_vec_mul(mat, cur_ans));
    /* e_k = x* - x,  r_k = b - Ax, r_k = Ae_k
    e_k = A^-1r_k -> ||e_k|| <= ||A^-1|| * ||r_k|| */
    if (((rhs - mat_vec_mul(mat, cur_ans)).get_euclidean_norm() / min_eigenval < epsilon)
          || (stop_by_max_iter && (cur_step > max_iter))) {
            break;
    }
  }
}

Vector<double> SimpleLinearSolver::solve() {
  simple_iteration_method();
  return cur_ans;
}
