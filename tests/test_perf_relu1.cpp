/**
 * Performance test for relu (Task: perf_relu1)
 *
 * Bug: Manual loop with branch prevents vectorization
 * Expected fix: Use std::transform with std::max
 *
 * Source problem: transform/55_transform_relu
 */

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <cmath>

#include "../perf_tasks/_templates/perf_harness.hpp"
#include "../perf_tasks/perf_relu1/baseline_snippet.hpp"
#include "../algorithms/transform/relu.hpp"

using namespace perf_harness;

const double EPSILON = 1e-9;

/**
 * Test: Verify relu produces correct results
 */
TEST(Perf_Relu1, Correctness) {
    // Test 1: Mixed positive and negative
    {
        std::vector<double> x = {-1.8, 24.0, 1.2, 0.0, -5.1, -0.2, 4.5};
        std::vector<double> expected = {0.0, 24.0, 1.2, 0.0, 0.0, 0.0, 4.5};

        relu(x);
        for (size_t i = 0; i < x.size(); ++i) {
            EXPECT_NEAR(x[i], expected[i], EPSILON);
        }
    }

    // Test 2: All positive
    {
        std::vector<double> x = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> expected = {1.0, 2.0, 3.0, 4.0};

        relu(x);
        for (size_t i = 0; i < x.size(); ++i) {
            EXPECT_NEAR(x[i], expected[i], EPSILON);
        }
    }

    // Test 3: All negative
    {
        std::vector<double> x = {-1.0, -2.0, -3.0, -4.0};
        std::vector<double> expected = {0.0, 0.0, 0.0, 0.0};

        relu(x);
        for (size_t i = 0; i < x.size(); ++i) {
            EXPECT_NEAR(x[i], expected[i], EPSILON);
        }
    }

    // Test 4: Zeros
    {
        std::vector<double> x = {0.0, 0.0, 0.0};
        std::vector<double> expected = {0.0, 0.0, 0.0};

        relu(x);
        for (size_t i = 0; i < x.size(); ++i) {
            EXPECT_NEAR(x[i], expected[i], EPSILON);
        }
    }
}

/**
 * Test: Performance gate - relu should be within 1.5x of baseline
 */
TEST(Perf_Relu1, Performance) {
    // Generate test data
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    const size_t n = 5000000;
    std::vector<double> test_data(n);
    for (size_t i = 0; i < n; ++i) {
        test_data[i] = dist(rng);
    }

    // Measure current implementation
    auto run_current = [&]() {
        std::vector<double> data_copy = test_data;
        relu(data_copy);
    };

    // Measure baseline implementation
    auto run_baseline = [&]() {
        std::vector<double> data_copy = test_data;
        relu_baseline(data_copy);
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
