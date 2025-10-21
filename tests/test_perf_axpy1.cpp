/**
 * Performance test for axpy (Task: perf_axpy1)
 *
 * Bug: Unnecessary pointer indirection for scalar prevents register optimization
 * Expected fix: Use alpha directly instead of through pointer
 *
 * Source problem: dense_la/03_dense_la_axpy
 */

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <cmath>

#include "../perf_tasks/_templates/perf_harness.hpp"
#include "../perf_tasks/perf_axpy1/baseline_snippet.hpp"
#include "../algorithms/dense_la/axpy.hpp"

using namespace perf_harness;

const double EPSILON = 1e-9;

/**
 * Test: Verify axpy produces correct results
 */
TEST(Perf_Axpy1, Correctness) {
    // Test 1: Simple case
    {
        std::vector<double> x = {1.0, -5.0, 2.0, 9.0};
        std::vector<double> y = {0.0, 4.0, 1.0, -1.0};
        std::vector<double> z(4);
        std::vector<double> expected = {2.0, -6.0, 5.0, 17.0};
        double alpha = 2.0;

        axpy(alpha, x, y, z);
        for (size_t i = 0; i < z.size(); ++i) {
            EXPECT_NEAR(z[i], expected[i], EPSILON);
        }
    }

    // Test 2: Alpha = 0
    {
        std::vector<double> x = {1.0, 2.0, 3.0};
        std::vector<double> y = {4.0, 5.0, 6.0};
        std::vector<double> z(3);
        std::vector<double> expected = {4.0, 5.0, 6.0};
        double alpha = 0.0;

        axpy(alpha, x, y, z);
        for (size_t i = 0; i < z.size(); ++i) {
            EXPECT_NEAR(z[i], expected[i], EPSILON);
        }
    }

    // Test 3: Alpha = 1
    {
        std::vector<double> x = {1.0, 2.0, 3.0};
        std::vector<double> y = {4.0, 5.0, 6.0};
        std::vector<double> z(3);
        std::vector<double> expected = {5.0, 7.0, 9.0};
        double alpha = 1.0;

        axpy(alpha, x, y, z);
        for (size_t i = 0; i < z.size(); ++i) {
            EXPECT_NEAR(z[i], expected[i], EPSILON);
        }
    }

    // Test 4: Negative alpha
    {
        std::vector<double> x = {1.0, 2.0, 3.0};
        std::vector<double> y = {4.0, 5.0, 6.0};
        std::vector<double> z(3);
        std::vector<double> expected = {3.0, 3.0, 3.0};
        double alpha = -1.0;

        axpy(alpha, x, y, z);
        for (size_t i = 0; i < z.size(); ++i) {
            EXPECT_NEAR(z[i], expected[i], EPSILON);
        }
    }
}

/**
 * Test: Performance gate - axpy should be within 1.5x of baseline
 */
TEST(Perf_Axpy1, Performance) {
    // Generate test data
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    const size_t n = 50000000;  // Larger size to show function call overhead
    std::vector<double> x(n), y(n), z(n);
    for (size_t i = 0; i < n; ++i) {
        x[i] = dist(rng);
        y[i] = dist(rng);
    }
    double alpha = 2.5;

    // Measure current implementation
    auto run_current = [&]() {
        std::vector<double> x_copy = x;
        std::vector<double> y_copy = y;
        std::vector<double> z_copy(n);
        axpy(alpha, x_copy, y_copy, z_copy);
    };

    // Measure baseline implementation
    auto run_baseline = [&]() {
        std::vector<double> x_copy = x;
        std::vector<double> y_copy = y;
        std::vector<double> z_copy(n);
        axpy_baseline(alpha, x_copy, y_copy, z_copy);
    };

    // Assert performance within 1.5x of baseline
    ASSERT_NO_THROW({
        assert_performance(run_current, run_baseline, 1.5, 5, 2);
    });
}

/**
 * Main function for standalone testing
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
