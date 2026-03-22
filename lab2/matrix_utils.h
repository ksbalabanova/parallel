//
// Created by Ксения Балабанова on 22.03.2026.
//

#ifndef PARALLEL_LABS_MATRIX_UTILS_H
#define PARALLEL_LABS_MATRIX_UTILS_H
#include <vector>

using matrix = std::vector<std::vector<float>>;

matrix create_matrix(int n);

void fill_matrix_random(matrix& a);

void zero_matrix(matrix& a);

bool are_equal(const matrix& a, const matrix& b, float eps = 1e-3f);

double calc_gflops(int n, double seconds);

#endif //PARALLEL_LABS_MATRIX_UTILS_H