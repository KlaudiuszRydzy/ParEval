/**
 * Baseline (correct) implementation of average from the ParEval benchmark.
 * This is the fast O(n) version without performance bugs.
 *
 * Source: drivers/cpp/benchmarks/reduce/27_reduce_average/baseline.hpp
 */

#pragma once
#include <vector>
#include <numeric>

/**
 * Return the average of the vector x.
 *
 * Examples:
 *   input: [1, 8, 4, 5, 1]
 *   output: 3.8
 *
 *   input: [2, 2, 2, 3]
 *   output: 2.25
 */
double average_baseline(const std::vector<double>& x) {
    return std::reduce(x.begin(), x.end(), 0.0) / static_cast<double>(x.size());
}
