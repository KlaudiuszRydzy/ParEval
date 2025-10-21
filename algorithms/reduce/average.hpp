/**
 * Average computation with performance bug
 *
 * Task: perf_avg1
 * Bug: Redundant sum() calls in loop causing O(n²) complexity
 * Expected behavior: Should compute average in O(n) time
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
 *
 * PERFORMANCE BUG: Lines 27-32 recompute sum on every iteration!
 * This causes O(n²) complexity instead of O(n).
 * FIX: Compute sum once before the loop.
 */
double average(const std::vector<double>& x) {
    // BUG: Computing sum inside the loop unnecessarily
    double total = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        // Redundant: recomputing sum from scratch each iteration!
        total = std::reduce(x.begin(), x.end(), 0.0);
    }
    return total / static_cast<double>(x.size());
}
