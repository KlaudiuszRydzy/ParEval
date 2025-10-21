/**
 * Array contains with performance bug
 *
 * Task: perf_contains1
 * Bug: Continues scanning after finding element
 * Expected behavior: Should short-circuit on first match
 */

#pragma once
#include <vector>

/**
 * Return true if the vector x contains the value `target`. Return false otherwise.
 *
 * Examples:
 *   input: x=[1, 8, 2, 6, 4, 6], target=3
 *   output: false
 *
 *   input: x=[1, 8, 2, 6, 4, 6], target=8
 *   output: true
 *
 * PERFORMANCE BUG: Lines 28-33 scan entire array even after finding match!
 * This is inefficient especially when target is near the beginning.
 * FIX: Return true immediately when found, or use std::find.
 */
bool contains(const std::vector<int>& x, int target) {
    // BUG: No early return - scans entire array even after finding match
    bool found = false;
    for (size_t i = 0; i < x.size(); ++i) {
        if (x[i] == target) {
            found = true;  // Should return here!
        }
    }
    return found;
}
