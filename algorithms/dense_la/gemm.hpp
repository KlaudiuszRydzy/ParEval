/**
 * GEMM with performance bug
 *
 * Task: perf_gemm1
 * Bug: Cache-unfriendly loop order (ijk instead of ikj)
 * Expected behavior: Should use cache-friendly loop ordering
 */

#pragma once
#include <vector>

/**
 * Multiply the matrix A by the matrix B. Store the results in the matrix C.
 * A is an MxK matrix, B is a KxN matrix, and C is a MxN matrix.
 * The matrices are stored in row-major.
 *
 * Example:
 *   input: A=[[1, -1, 2], [0, -2, 1]] B=[[4, 1], [-1, 0], [2, 2]]
 *   output: C=[[9, 5], [4, 2]]
 *
 * PERFORMANCE BUG: Lines 30-36 use ijk loop order!
 * This causes cache-unfriendly access pattern with poor spatial locality.
 * Matrix B is accessed in column-major fashion (stride N) causing cache misses.
 * FIX: Reorder loops to ikj for better cache performance.
 */
void gemm(const std::vector<double>& A, const std::vector<double>& B, std::vector<double>& C,
          size_t M, size_t K, size_t N) {
    // BUG: ijk loop order - cache-unfriendly!
    // B is accessed in column-major order causing many cache misses
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < K; ++k) {
                // Each iteration accesses B[k*N + j] with stride N - poor locality!
                C[i*N + j] += A[i*K + k] * B[k*N + j];
            }
        }
    }
}
