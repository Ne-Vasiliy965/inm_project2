#ifndef SIMPLE_LINEAR_SOLVER
#define SIMPLE_LINEAR_SOLVER

#include "vector_matrix.h"

  /*Solver for only symmetric positively defined matrix.
    In this version you need to give the estimation of 
    max and min eigenvalues of matrix.
    You should give a good estimate for the min eigenvalue.
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
  void simple_iteration_method();

 public:
  SimpleLinearSolver(double epsilon, const Matrix<double>& mat, const Vector<double>& rhs,
                     double min_ei, double max_ei, bool stop_by_max_iter = false, 
                     size_t max_iter = 0);

  SimpleLinearSolver(double epsilon, const Matrix<double>& mat, const Vector<double>& rhs,
                     Vector<double> starting_point, double min_ei, double max_ei,
                     bool stop_by_max_iter = false, size_t max_iter = 0);

  ~SimpleLinearSolver() = default;

  Vector<double> solve();
};
#endif