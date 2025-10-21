/**
 * Baseline (correct) implementation of prefix_sum from the ParEval benchmark.
 * This is the fast O(n) version using std::inclusive_scan.
 *
 * Source: drivers/cpp/benchmarks/scan/30_scan_prefix_sum/baseline.hpp
 */

#pragma once
#include <numeric>
#include <vector>

/**
 * Compute the prefix sum of the vector x into output.
 *
 * Example:
 *   input: [1, 7, 4, 6, 6, 2]
 *   output: [1, 8, 12, 18, 24, 26]
 */
void prefix_sum_baseline(const std::vector<double>& x, std::vector<double>& output) {
    std::inclusive_scan(x.begin(), x.end(), output.begin());
}
