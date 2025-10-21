/**
 * Baseline (correct) implementation of AXPY from the ParEval benchmark.
 * This is the fast version with cache-friendly access.
 *
 * Source: drivers/cpp/benchmarks/dense_la/03_dense_la_axpy/baseline.hpp
 */

#pragma once
#include <vector>

/**
 * Compute z = alpha*x+y where x and y are vectors. Store the result in z.
 *
 * Example:
 *   input: x=[1, -5, 2, 9] y=[0, 4, 1, -1] alpha=2
 *   output: z=[2, -6, 5, 17]
 */
void axpy_baseline(double alpha, const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& z) {
    for (size_t i = 0; i < x.size(); ++i) {
        z[i] = alpha * x[i] + y[i];
    }
}
