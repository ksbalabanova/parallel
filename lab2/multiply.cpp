#include "multiply.h"
#include <algorithm>

matrix transpose_matrix(const matrix& b) {
    int n = b.size();
    matrix bt = create_matrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            bt[j][i] = b[i][j];
        }
    }
    return bt;
}

matrix mul_classic(const matrix& a, const matrix& b) {
    int n = a.size();
    matrix c = create_matrix(n);
    zero_matrix(c);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
    return c;
}

matrix mul_transpose(const matrix& a, const matrix& b) {
    int n = a.size();
    matrix c = create_matrix(n);
    matrix bt = transpose_matrix(b);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += a[i][k] * bt[j][k];
            }
            c[i][j] = sum;
        }
    }
    return c;
}

matrix mul_buffered(const matrix& a, const matrix& b, int m_unroll) {
    int n = a.size();
    matrix c = create_matrix(n);
    zero_matrix(c);

    std::vector<float> tmp(n);

    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            tmp[k] = b[k][j];
        }

        for (int i = 0; i < n; i++) {
            float sum = 0.0f;

            for (int k = 0; k < n; k++) {
                sum += a[i][k] * tmp[k];
            }

            c[i][j] = sum;
        }
    }

    return c;
}

matrix mul_blocked(const matrix& a, const matrix& b, int block_size, int m_unroll) {
    (void)m_unroll;
    int n = a.size();
    matrix c = create_matrix(n);
    zero_matrix(c);
    for (int ii = 0; ii < n; ii += block_size) {
        for (int jj = 0; jj < n; jj += block_size) {
            for (int kk = 0; kk < n; kk += block_size) {
                int i_max = std::min(ii + block_size, n);
                int j_max = std::min(jj + block_size, n);
                int k_max = std::min(kk + block_size, n);
                for (int i = ii; i < i_max; i++) {
                    for (int j = jj; j < j_max; j++) {
                        for (int k = kk; k < k_max; k++) {
                            c[i][j] += a[i][k] * b[k][j];
                        }
                    }
                }
            }
        }
    }
    return c;
}