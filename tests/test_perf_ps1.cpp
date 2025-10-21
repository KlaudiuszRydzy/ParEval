/**
 * Performance test for prefix_sum (Task: perf_ps1)
 *
 * Bug: Nested loop for prefix sum causing O(n²) complexity
 * Expected fix: Use std::inclusive_scan from <numeric>
 *
 * Source problem: scan/30_scan_prefix_sum
 */

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <cmath>

#include "../perf_tasks/_templates/perf_harness.hpp"
#include "../perf_tasks/perf_ps1/baseline_snippet.hpp"
#include "../algorithms/scan/prefix_sum.hpp"

using namespace perf_harness;

const double EPSILON = 1e-9;

/**
 * Test: Verify prefix_sum produces correct results
 */
TEST(Perf_Ps1, Correctness) {
    // Test 1: Simple case
    {
        std::vector<double> x = {1.0, 7.0, 4.0, 6.0, 6.0, 2.0};
        std::vector<double> output(6);
        std::vector<double> expected = {1.0, 8.0, 12.0, 18.0, 24.0, 26.0};

        prefix_sum(x, output);
        for (size_t i = 0; i < output.size(); ++i) {
            EXPECT_NEAR(output[i], expected[i], EPSILON);
        }
    }

    // Test 2: All ones
    {
        std::vector<double> x(10, 1.0);
        std::vector<double> output(10);

        prefix_sum(x, output);
        for (size_t i = 0; i < output.size(); ++i) {
            EXPECT_NEAR(output[i], static_cast<double>(i + 1), EPSILON);
        }
    }

    // Test 3: Single element
    {
        std::vector<double> x = {42.0};
        std::vector<double> output(1);

        prefix_sum(x, output);
        EXPECT_NEAR(output[0], 42.0, EPSILON);
    }

    // Test 4: Negative numbers
    {
        std::vector<double> x = {1.0, -2.0, 3.0, -4.0};
        std::vector<double> output(4);
        std::vector<double> expected = {1.0, -1.0, 2.0, -2.0};

        prefix_sum(x, output);
        for (size_t i = 0; i < output.size(); ++i) {
            EXPECT_NEAR(output[i], expected[i], EPSILON);
        }
    }
}

/**
 * Test: Performance gate - prefix_sum should be within 1.5x of baseline
 */
TEST(Perf_Ps1, Performance) {
    // Generate test data
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    const size_t n = 10000;  // O(n²) algorithm needs smaller n
    std::vector<double> test_data(n);
    for (size_t i = 0; i < n; ++i) {
        test_data[i] = dist(rng);
    }

    // Measure current implementation
    auto run_current = [&]() {
        std::vector<double> data_copy = test_data;
        std::vector<double> output(n);
        prefix_sum(data_copy, output);
        // Prevent optimization
        volatile double sum = output[n/2];
        (void)sum;
    };

    // Measure baseline implementation
    auto run_baseline = [&]() {
        std::vector<double> data_copy = test_data;
        std::vector<double> output(n);
        prefix_sum_baseline(data_copy, output);
        // Prevent optimization
        volatile double sum = output[n/2];
        (void)sum;
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
