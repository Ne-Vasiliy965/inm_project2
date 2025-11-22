#include "vector_matrix.h"
#include "linear_system_solver.h"

//  --------------Solver Implementation----------------

SimpleLinearSolver::SimpleLinearSolver(
                     double epsilon,
                     const Matrix<double>& mat,
                     const Vector<double>& rhs,
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
  this->mat = mat;
  this->rhs = rhs;
  size_t dim = rhs.get_size();
  std::vector<double> tmp(dim, 0);
  cur_ans = Vector(tmp);
}

SimpleLinearSolver::SimpleLinearSolver(
                     double epsilon,
                     const Matrix<double>& mat,
                     const Vector<double>& rhs,
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
  this->mat = mat;
  this->rhs = rhs;
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
