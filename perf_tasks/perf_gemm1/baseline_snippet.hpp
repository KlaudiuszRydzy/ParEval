/**
 * Baseline (correct) implementation of GEMM from the ParEval benchmark.
 * This is the cache-friendly version with ikj loop order.
 *
 * Source: drivers/cpp/benchmarks/dense_la/02_dense_la_gemm/baseline.hpp
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
 */
void gemm_baseline(const std::vector<double>& A, const std::vector<double>& B, std::vector<double>& C,
                   size_t M, size_t K, size_t N) {
    // Cache-friendly ikj order
    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            for (size_t j = 0; j < N; ++j) {
                C[i*N + j] += A[i*K + k] * B[k*N + j];
            }
        }
    }
}
