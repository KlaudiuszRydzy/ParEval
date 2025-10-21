/**
 * Performance test for gemm (Task: perf_gemm1)
 *
 * Bug: ijk loop order causes cache-unfriendly access pattern
 * Expected fix: Reorder loops to ikj for better cache locality
 *
 * Source problem: dense_la/02_dense_la_gemm
 */

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <cmath>

#include "../perf_tasks/_templates/perf_harness.hpp"
#include "../perf_tasks/perf_gemm1/baseline_snippet.hpp"
#include "../algorithms/dense_la/gemm.hpp"

using namespace perf_harness;

const double EPSILON = 1e-6;

/**
 * Test: Verify gemm produces correct results
 */
TEST(Perf_Gemm1, Correctness) {
    // Test 1: Simple 2x3 * 3x2 case
    {
        std::vector<double> A = {1.0, -1.0, 2.0, 0.0, -2.0, 1.0};
        std::vector<double> B = {4.0, 1.0, -1.0, 0.0, 2.0, 2.0};
        std::vector<double> C(4, 0.0);
        std::vector<double> expected = {9.0, 5.0, 4.0, 2.0};

        gemm(A, B, C, 2, 3, 2);
        for (size_t i = 0; i < C.size(); ++i) {
            EXPECT_NEAR(C[i], expected[i], EPSILON);
        }
    }

    // Test 2: Identity matrix
    {
        std::vector<double> A = {1.0, 2.0, 3.0, 4.0};  // 2x2
        std::vector<double> B = {1.0, 0.0, 0.0, 1.0};  // 2x2 identity
        std::vector<double> C(4, 0.0);
        std::vector<double> expected = {1.0, 2.0, 3.0, 4.0};

        gemm(A, B, C, 2, 2, 2);
        for (size_t i = 0; i < C.size(); ++i) {
            EXPECT_NEAR(C[i], expected[i], EPSILON);
        }
    }

    // Test 3: Zero matrix
    {
        std::vector<double> A = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> B = {0.0, 0.0, 0.0, 0.0};
        std::vector<double> C(4, 0.0);
        std::vector<double> expected = {0.0, 0.0, 0.0, 0.0};

        gemm(A, B, C, 2, 2, 2);
        for (size_t i = 0; i < C.size(); ++i) {
            EXPECT_NEAR(C[i], expected[i], EPSILON);
        }
    }
}

/**
 * Test: Performance gate - gemm should be within 1.5x of baseline
 */
TEST(Perf_Gemm1, Performance) {
    // Generate test data - moderate size matrices
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    const size_t M = 256, K = 256, N = 256;
    std::vector<double> A(M * K), B(K * N), C(M * N, 0.0);

    for (size_t i = 0; i < M * K; ++i) {
        A[i] = dist(rng);
    }
    for (size_t i = 0; i < K * N; ++i) {
        B[i] = dist(rng);
    }

    // Measure current implementation
    auto run_current = [&]() {
        std::vector<double> A_copy = A;
        std::vector<double> B_copy = B;
        std::vector<double> C_copy(M * N, 0.0);
        gemm(A_copy, B_copy, C_copy, M, K, N);
    };

    // Measure baseline implementation
    auto run_baseline = [&]() {
        std::vector<double> A_copy = A;
        std::vector<double> B_copy = B;
        std::vector<double> C_copy(M * N, 0.0);
        gemm_baseline(A_copy, B_copy, C_copy, M, K, N);
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
