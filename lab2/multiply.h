#ifndef PARALLEL_LABS_MULTIPLY_H
#define PARALLEL_LABS_MULTIPLY_H
#include "matrix_utils.h"

matrix transpose_matrix(const matrix& b);

matrix mul_classic(const matrix& a, const matrix& b);

matrix mul_transpose(const matrix& a, const matrix& b);

matrix mul_buffered(const matrix& a, const matrix& b, int m_unroll = 1);

matrix mul_blocked(const matrix& a, const matrix& b, int block_size, int m_unroll = 1);
#endif //PARALLEL_LABS_MULTIPLY_H