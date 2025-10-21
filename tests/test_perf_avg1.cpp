/**
 * Performance test for average computation (Task: perf_avg1)
 *
 * Bug: Redundant sum() calls in average loop causing O(n²) complexity
 * Expected fix: Compute sum once, then divide by size
 *
 * Source problem: reduce/27_reduce_average
 */

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <cmath>

#include "../perf_tasks/_templates/perf_harness.hpp"
#include "../perf_tasks/perf_avg1/baseline_snippet.hpp"
#include "../algorithms/reduce/average.hpp"  // Current (buggy) implementation

using namespace perf_harness;

const double EPSILON = 1e-9;

/**
 * Test: Verify average produces correct results
 */
TEST(Perf_Avg1, Correctness) {
    // Test 1: Simple case
    {
        std::vector<double> x = {1.0, 8.0, 4.0, 5.0, 1.0};
        double result = average(x);
        EXPECT_NEAR(result, 3.8, EPSILON);
    }

    // Test 2: All same values
    {
        std::vector<double> x = {2.0, 2.0, 2.0, 3.0};
        double result = average(x);
        EXPECT_NEAR(result, 2.25, EPSILON);
    }

    // Test 3: Single element
    {
        std::vector<double> x = {42.0};
        double result = average(x);
        EXPECT_NEAR(result, 42.0, EPSILON);
    }

    // Test 4: Negative numbers
    {
        std::vector<double> x = {-1.0, -2.0, -3.0, -4.0};
        double result = average(x);
        EXPECT_NEAR(result, -2.5, EPSILON);
    }

    // Test 5: Mixed positive and negative
    {
        std::vector<double> x = {-10.0, 10.0, -5.0, 5.0};
        double result = average(x);
        EXPECT_NEAR(result, 0.0, EPSILON);
    }

    // Test 6: Larger dataset
    {
        std::vector<double> x(1000, 5.0);
        double result = average(x);
        EXPECT_NEAR(result, 5.0, EPSILON);
    }
}

/**
 * Test: Performance gate - average should be within 1.5x of baseline
 *
 * Compares current implementation against the correct baseline.
 * With the bug, current will be much slower (5000x+).
 * After fixing, current should match baseline speed (within 1.5x).
 */
TEST(Perf_Avg1, Performance) {
    // Generate test data
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    const size_t n = 50000;
    std::vector<double> test_data(n);
    for (size_t i = 0; i < n; ++i) {
        test_data[i] = dist(rng);
    }

    // Measure current implementation
    auto run_current = [&]() {
        std::vector<double> data_copy = test_data;
        volatile double result = average(data_copy);
        (void)result;  // Prevent optimization
    };

    // Measure baseline implementation
    auto run_baseline = [&]() {
        std::vector<double> data_copy = test_data;
        volatile double result = average_baseline(data_copy);
        (void)result;  // Prevent optimization
    };

    // Assert performance within 1.5x of baseline
    ASSERT_NO_THROW({
        assert_performance(run_current, run_baseline, 1.5, 3, 1);
    });
}

/**
 * Main function for standalone testing
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
