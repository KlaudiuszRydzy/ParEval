/**
 * Prefix sum computation with performance bug
 *
 * Task: perf_ps1
 * Bug: Manual nested loop instead of std::inclusive_scan causing O(n²) complexity
 * Expected behavior: Should compute prefix sum in O(n) time
 */

#pragma once
#include <vector>

/**
 * Compute the prefix sum of the vector x into output.
 *
 * Example:
 *   input: [1, 7, 4, 6, 6, 2]
 *   output: [1, 8, 12, 18, 24, 26]
 *
 * PERFORMANCE BUG: Lines 23-30 use nested loop for prefix sum!
 * This causes O(n²) complexity instead of O(n).
 * FIX: Use std::inclusive_scan from <numeric>.
 */
// Prevent inlining to make bug observable
__attribute__((noinline))
static double compute_sum_up_to(const std::vector<double>& x, size_t end_idx) {
    double sum = 0.0;
    for (size_t j = 0; j <= end_idx; ++j) {
        sum += x[j];
    }
    return sum;
}

void prefix_sum(const std::vector<double>& x, std::vector<double>& output) {
    // BUG: Manually recomputing sum from scratch for each position via function call
    // The __attribute__((noinline)) prevents compiler from optimizing this away
    for (size_t i = 0; i < x.size(); ++i) {
        // Redundant: recomputing entire sum from 0 to i for every i
        output[i] = compute_sum_up_to(x, i);
    }
}
