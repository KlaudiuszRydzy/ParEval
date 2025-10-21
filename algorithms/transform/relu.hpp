/**
 * ReLU with performance bug
 *
 * Task: perf_relu1
 * Bug: Manual loop with branch instead of std::transform
 * Expected behavior: Should use vectorizable std::transform
 */

#pragma once
#include <vector>

/**
 * Compute the ReLU function on every element of x. Elements less than zero become zero,
 * while elements greater than zero stay the same.
 *
 * Example:
 *   input: [-1.8, 24.0, 1.2, 0.0, -5.1, -0.2, 4.5]
 *   output: [0, 24.0, 1.2, 0, 0, 0, 4.5]
 *
 * PERFORMANCE BUG: Lines 27-33 use manual loop with branch prediction issues!
 * The if-else in the loop prevents compiler auto-vectorization.
 * FIX: Use std::transform with std::max for better vectorization.
 */
void relu(std::vector<double>& x) {
    // BUG: Manual loop with branch - hard to vectorize
    for (size_t i = 0; i < x.size(); ++i) {
        if (x[i] < 0.0) {
            x[i] = 0.0;
        } else {
            x[i] = x[i];  // Redundant but mirrors the logic
        }
    }
}
