/**
 * AXPY with performance bug
 *
 * Task: perf_axpy1
 * Bug: Accesses alpha via pointer dereference in loop
 * Expected behavior: Should access alpha directly (scalar in register)
 */

#pragma once
#include <vector>

/**
 * Compute z = alpha*x+y where x and y are vectors. Store the result in z.
 *
 * Example:
 *   input: x=[1, -5, 2, 9] y=[0, 4, 1, -1] alpha=2
 *   output: z=[2, -6, 5, 17]
 *
 * PERFORMANCE BUG: Lines 26-30 access alpha via pointer dereference!
 * This prevents compiler from keeping alpha in a register.
 * FIX: Use alpha directly instead of through pointer.
 */
// Prevent inlining and optimization
__attribute__((noinline))
static double load_alpha(const double* ptr) {
    // Volatile read prevents compiler from caching the value
    volatile const double* vptr = ptr;
    return *vptr;
}

void axpy(double alpha, const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& z) {
    // BUG: Unnecessary function call to load alpha on every iteration
    const double* alpha_ptr = &alpha;
    for (size_t i = 0; i < x.size(); ++i) {
        // Function call in loop prevents register optimization
        z[i] = load_alpha(alpha_ptr) * x[i] + y[i];
    }
}
