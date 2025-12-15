#include "linear_system_solver.h"
#include "vector_matrix.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

// Вспомогательная функция для сравнения векторов с заданной точностью
bool vectors_equal(const Vector<double>& v1, const Vector<double>& v2, double epsilon) {
    if (v1.get_size() != v2.get_size()) return false;
    for (size_t i = 0; i < v1.get_size(); ++i) {
        if (std::abs(v1[i] - v2[i]) > epsilon) return false;
    }
    return true;
}

// Вспомогательная функция для проверки решения Ax = b
bool check_solution(const Matrix<double>& A, const Vector<double>& x, 
                   const Vector<double>& b, double epsilon, double min_eigenval) {
    Vector<double> Ax = mat_vec_mul(A, x);
    Vector<double> residual = b - Ax;
    return residual.get_euclidean_norm() / min_eigenval < epsilon;
}

void test_1x1_system() {
    std::cout << "Testing 1x1 system..." << std::endl;
    
    // A = [[4]], b = [8], solution: x = [2]
    std::vector<std::vector<double>> a = {{4.0}};
    Matrix<double> A(a);
    Vector<double> b({8.0});
    
    SimpleLinearSolver solver(1e-10, A, b, 4.0, 4.0);
    Vector<double> solution = solver.solve();
    
    Vector<double> expected({2.0});
    assert(vectors_equal(solution, expected, 1e-10));
    assert(check_solution(A, solution, b, 1e-10, 4.0));
    // std::cout << solution;
    std::cout << "1x1 system test passed!" << std::endl;
}

void test_2x2_diagonal() {
    std::cout << "Testing 2x2 diagonal system..." << std::endl;
    
    // A = [[4, 0], [0, 9]], b = [8, 18], solution: x = [2, 2]
    Matrix<double> A({{4.0, 0.0}, {0.0, 9.0}});
    Vector<double> b({8.0, 18.0});
    
    SimpleLinearSolver solver(1e-10, A, b, 4.0, 9.0);
    Vector<double> solution = solver.solve();
    
    Vector<double> expected({2.0, 2.0});
    assert(vectors_equal(solution, expected, 1e-10));
    assert(check_solution(A, solution, b, 1e-10, 4.0));
    // std::cout << solution;
    std::cout << "2x2 diagonal system test passed!" << std::endl;
}

void test_2x2_full() {
    std::cout << "Testing 2x2 full system..." << std::endl;
    
    // A = [[5, 2], [2, 5]] - симметричная положительно определенная
    // Собственные значения: 3 и 7
    // b = [9, 12], решение: x = [1, 2]
    Matrix<double> A({{5.0, 2.0}, {2.0, 5.0}});
    Vector<double> b({9.0, 12.0});
    
    SimpleLinearSolver solver(1e-10, A, b, 3.0, 7.0);
    Vector<double> solution = solver.solve();
    
    Vector<double> expected({1.0, 2.0});
    assert(vectors_equal(solution, expected, 1e-10));
    assert(check_solution(A, solution, b, 1e-10, 3.0));
    // std::cout << solution;
    std::cout << "2x2 full system test passed!" << std::endl;
}

void test_3x3_system() {
    std::cout << "Testing 3x3 system..." << std::endl;
    
    // A = [[10, 2, 1], [2, 10, 2], [1, 2, 10]] - симметричная положительно определенная
    // Примерные собственные значения: min ≈ 6.5, max ≈ 13.5
    Matrix<double> A({{10.0, 2.0, 1.0}, 
                     {2.0, 10.0, 2.0}, 
                     {1.0, 2.0, 10.0}});
    
    // Решение: x = [1, 2, 3]
    Vector<double> x({1.0, 2.0, 3.0});
    Vector<double> b = mat_vec_mul(A, x);
    
    SimpleLinearSolver solver(1e-8, A, b, 6.5, 13.5);
    Vector<double> solution = solver.solve();
    assert(vectors_equal(solution, x, 1e-7));
    assert(check_solution(A, solution, b, 1e-8, 6.5));
    std::cout << "3x3 system test passed!" << std::endl;
}

// void test_ill_conditioned() {
//     std::cout << "Testing ill-conditioned system..." << std::endl;
    
//     // Матрица Гильберта 4x4 - известная плохо обусловленная матрица
//     // но все же положительно определенная
//     Matrix<double> A({{1.0, 1.0/2, 1.0/3, 1.0/4},
//                      {1.0/2, 1.0/3, 1.0/4, 1.0/5},
//                      {1.0/3, 1.0/4, 1.0/5, 1.0/6},
//                      {1.0/4, 1.0/5, 1.0/6, 1.0/7}});
    
//     Vector<double> x({1.0, 1.0, 1.0, 1.0});
//     Vector<double> b = mat_vec_mul(A, x);
    
//     // Для матрицы Гильберта собственные значения известны приблизительно
//     SimpleLinearSolver solver(1e-3, A, b, 0.0001, 1.5);
//     Vector<double> solution = solver.solve();
    
//     assert(check_solution(A, solution, b, 1e-3, 0.0001));
//     std::cout << "Ill-conditioned system test passed!" << std::endl;
// }

void test_with_initial_guess() {
    std::cout << "Testing with initial guess..." << std::endl;
    
    Matrix<double> A({{8.0, 1.0}, {1.0, 8.0}});
    Vector<double> b({10.0, 10.0});
    Vector<double> initial_guess({0.5, 0.5});
    
    SimpleLinearSolver solver(1e-10, A, b, initial_guess, 7.0, 9.0);
    Vector<double> solution = solver.solve();
    
    Vector<double> expected({10.0/7.0, 10.0/7.0}); // Примерное решение
    assert(check_solution(A, solution, b, 1e-10, 7.0));
    std::cout << "Initial guess test passed!" << std::endl;
}

void test_max_iterations() {
    std::cout << "Testing max iterations..." << std::endl;
    
    Matrix<double> A({{100.0, 1.0}, {1.0, 100.0}});
    Vector<double> b({101.0, 101.0});
    
    // Используем очень маленькое количество итераций
    SimpleLinearSolver solver(1e-10, A, b, 99.0, 101.0, true, 5);
    Vector<double> solution = solver.solve();
    
    // Проверяем, что решение не сошлось к точному, но метод отработал
    std::cout << "Max iterations test completed!" << std::endl;
}

void test_identity_matrix() {
    std::cout << "Testing identity matrix..." << std::endl;
    
    // A = I, b = [1, 2, 3], решение: x = [1, 2, 3]
    Matrix<double> A({{1.0, 0.0, 0.0},
                     {0.0, 1.0, 0.0},
                     {0.0, 0.0, 1.0}});
    Vector<double> b({1.0, 2.0, 3.0});
    
    SimpleLinearSolver solver(1e-10, A, b, 1.0, 1.0);
    Vector<double> solution = solver.solve();
    
    assert(vectors_equal(solution, b, 1e-10));
    assert(check_solution(A, solution, b, 1e-10, 1.0));
    std::cout << "Identity matrix test passed!" << std::endl;
}

int main() {
    try {
        test_1x1_system();
        test_2x2_diagonal();
        test_2x2_full();
        test_3x3_system();
        // test_ill_conditioned();
        test_with_initial_guess();
        test_max_iterations();
        test_identity_matrix();
        
        std::cout << "\nAll tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}