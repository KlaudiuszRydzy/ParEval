/**
 * Reusable performance testing utilities for the C++ performance bug suite.
 *
 * Provides statistical timing measurements with relative speedup gates.
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace perf_harness {

/**
 * Performance measurement results
 */
struct PerformanceResult {
    std::vector<double> times;  // All timing measurements (seconds)
    double median;              // Median time
    double mean;                // Mean time
    double min;                 // Minimum time
    double max;                // Maximum time
    double std;                 // Standard deviation
};

/**
 * Measure function performance over multiple runs.
 *
 * @param func Function to benchmark
 * @param n_runs Number of measurement runs
 * @param warmup Number of warmup runs (not measured)
 * @return Performance measurement results
 */
template <typename Func>
PerformanceResult measure_performance(Func func, int n_runs = 10, int warmup = 2) {
    using namespace std::chrono;

    // Warmup runs
    for (int i = 0; i < warmup; ++i) {
        func();
    }

    // Measurement runs
    std::vector<double> times;
    times.reserve(n_runs);

    for (int i = 0; i < n_runs; ++i) {
        auto start = high_resolution_clock::now();
        func();
        auto end = high_resolution_clock::now();

        double elapsed = duration_cast<duration<double>>(end - start).count();
        times.push_back(elapsed);
    }

    // Compute statistics
    PerformanceResult result;
    result.times = times;

    // Median
    std::vector<double> sorted_times = times;
    std::sort(sorted_times.begin(), sorted_times.end());
    if (sorted_times.size() % 2 == 0) {
        result.median = (sorted_times[sorted_times.size()/2 - 1] +
                        sorted_times[sorted_times.size()/2]) / 2.0;
    } else {
        result.median = sorted_times[sorted_times.size()/2];
    }

    // Mean
    result.mean = std::accumulate(times.begin(), times.end(), 0.0) / times.size();

    // Min/Max
    result.min = *std::min_element(times.begin(), times.end());
    result.max = *std::max_element(times.begin(), times.end());

    // Standard deviation
    double sq_sum = 0.0;
    for (double t : times) {
        sq_sum += (t - result.mean) * (t - result.mean);
    }
    result.std = times.size() > 1 ? std::sqrt(sq_sum / times.size()) : 0.0;

    return result;
}

/**
 * Assert that current implementation is within acceptable slowdown of baseline.
 *
 * @param current_func The current (potentially buggy) implementation
 * @param baseline_func The correct baseline implementation
 * @param max_slowdown Maximum acceptable slowdown factor (default 1.5x)
 * @param n_runs Number of measurement runs
 * @param warmup Number of warmup runs
 * @throws std::runtime_error if slowdown exceeds threshold
 */
template <typename Func1, typename Func2>
void assert_performance(
    Func1 current_func,
    Func2 baseline_func,
    double max_slowdown = 1.5,
    int n_runs = 3,
    int warmup = 1
) {
    // Measure current implementation
    auto current_perf = measure_performance(current_func, n_runs, warmup);

    // Measure baseline implementation
    auto baseline_perf = measure_performance(baseline_func, n_runs, warmup);

    // Calculate slowdown factor
    double current_time = current_perf.median;
    double baseline_time = baseline_perf.median;
    double slowdown_factor = current_time / baseline_time;

    // Check performance gate
    if (slowdown_factor >= max_slowdown) {
        char msg[512];
        snprintf(msg, sizeof(msg),
                "Performance test failed: slowdown %.2fx >= required %.2fx "
                "(current: %.4fs, baseline: %.4fs)",
                slowdown_factor, max_slowdown, current_time, baseline_time);
        throw std::runtime_error(msg);
    }
}

/**
 * Compare performance and print results (for manual testing).
 *
 * @param current_func The current implementation
 * @param baseline_func The baseline implementation
 * @param name Name of the function being tested
 * @param n_runs Number of measurement runs
 * @param warmup Number of warmup runs
 * @return Slowdown factor
 */
template <typename Func1, typename Func2>
double compare_performance(
    Func1 current_func,
    Func2 baseline_func,
    const char* name = "function",
    int n_runs = 3,
    int warmup = 1
) {
    auto current_perf = measure_performance(current_func, n_runs, warmup);
    auto baseline_perf = measure_performance(baseline_func, n_runs, warmup);

    double slowdown = current_perf.median / baseline_perf.median;

    printf("%s performance:\n", name);
    printf("  Current:  %.4fs (median of %d runs)\n", current_perf.median, n_runs);
    printf("  Baseline: %.4fs (median of %d runs)\n", baseline_perf.median, n_runs);
    printf("  Slowdown: %.2fx\n", slowdown);

    return slowdown;
}

} // namespace perf_harness
