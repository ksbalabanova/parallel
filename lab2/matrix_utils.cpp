//
// Created by Ксения Балабанова on 22.03.2026.
//
#include "matrix_utils.h"
#include <random>
#include <cmath>

matrix create_matrix(int n) {
    return matrix(n, std::vector<float>(n));
}

void fill_matrix_random(matrix& a) {
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.0f, 10.0f);
    int n = a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = dist(gen);
        }
    }
}

void zero_matrix(matrix& a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = 0.0f;
        }
    }
}

bool are_equal(const matrix& a, const matrix& b, float eps) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (std::fabs(a[i][j] - b[i][j]) > eps) {
                return false;
            }
        }
    }
    return true;
}

double calc_gflops(int n, double seconds) {
    double operations = 2.0 * n * n * n;
    return operations / seconds / 1e9;
}