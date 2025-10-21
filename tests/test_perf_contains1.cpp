/**
 * Performance test for contains (Task: perf_contains1)
 *
 * Bug: No early return - scans entire array even after finding match
 * Expected fix: Return true immediately when found, or use std::find
 *
 * Source problem: search/36_search_check_if_array_contains_value
 */

#include <gtest/gtest.h>
#include <vector>
#include <random>

#include "../perf_tasks/_templates/perf_harness.hpp"
#include "../perf_tasks/perf_contains1/baseline_snippet.hpp"
#include "../algorithms/search/contains.hpp"

using namespace perf_harness;

/**
 * Test: Verify contains produces correct results
 */
TEST(Perf_Contains1, Correctness) {
    // Test 1: Element not present
    {
        std::vector<int> x = {1, 8, 2, 6, 4, 6};
        EXPECT_FALSE(contains(x, 3));
    }

    // Test 2: Element present
    {
        std::vector<int> x = {1, 8, 2, 6, 4, 6};
        EXPECT_TRUE(contains(x, 8));
    }

    // Test 3: First element
    {
        std::vector<int> x = {1, 8, 2, 6, 4, 6};
        EXPECT_TRUE(contains(x, 1));
    }

    // Test 4: Last element
    {
        std::vector<int> x = {1, 8, 2, 6, 4, 6};
        EXPECT_TRUE(contains(x, 6));
    }

    // Test 5: Single element - present
    {
        std::vector<int> x = {42};
        EXPECT_TRUE(contains(x, 42));
    }

    // Test 6: Single element - not present
    {
        std::vector<int> x = {42};
        EXPECT_FALSE(contains(x, 7));
    }

    // Test 7: Empty array
    {
        std::vector<int> x;
        EXPECT_FALSE(contains(x, 42));
    }
}

/**
 * Test: Performance gate - contains should be within 1.5x of baseline
 *
 * Note: Bug is most impactful when target is near beginning of array.
 */
TEST(Perf_Contains1, Performance) {
    // Generate test data - target is at position 100 (early in array)
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1000, 2000);

    const size_t n = 1000000;
    std::vector<int> test_data(n);
    for (size_t i = 0; i < n; ++i) {
        test_data[i] = dist(rng);
    }
    test_data[100] = 42;  // Target is near beginning

    // Measure current implementation (searches for early element)
    auto run_current = [&]() {
        volatile bool result = contains(test_data, 42);
        (void)result;
    };

    // Measure baseline implementation
    auto run_baseline = [&]() {
        volatile bool result = contains_baseline(test_data, 42);
        (void)result;
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
