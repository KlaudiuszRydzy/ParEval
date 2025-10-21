/**
 * Baseline (correct) implementation of contains from the ParEval benchmark.
 * This is the fast version using std::find.
 *
 * Source: drivers/cpp/benchmarks/search/36_search_check_if_array_contains_value/baseline.hpp
 */

#pragma once
#include <vector>
#include <algorithm>

/**
 * Return true if the vector x contains the value `target`. Return false otherwise.
 *
 * Examples:
 *   input: x=[1, 8, 2, 6, 4, 6], target=3
 *   output: false
 *
 *   input: x=[1, 8, 2, 6, 4, 6], target=8
 *   output: true
 */
bool contains_baseline(const std::vector<int>& x, int target) {
    return std::find(x.begin(), x.end(), target) != x.end();
}
