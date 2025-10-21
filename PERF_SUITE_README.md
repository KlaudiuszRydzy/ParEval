# C++ Performance Bug Test Suite for SWE-Agent

## Overview

This repository branch (`perf-bugs-suite`) contains **6 C++ performance bug tasks** based on the [ParEval](https://github.com/parallelcodefoundry/ParEval) benchmark for evaluating LLMs on parallel code.

## Repository Structure

```
perf-bugs-suite branch:
├── algorithms/               # Algorithm implementations (with bugs injected)
│   ├── reduce/
│   ├── scan/
│   ├── search/
│   ├── transform/
│   └── dense_la/
├── tests/                    # Performance test harnesses with baseline comparison
│   ├── Makefile
│   └── test_perf_*.cpp
├── perf_tasks/              # Task metadata and baseline snippets
│   ├── _templates/perf_harness.hpp  # Reusable timing utilities
│   ├── perf_*/baseline_snippet.hpp  # Correct implementations
│   └── perf_*/problem_*.md          # Problem statements (3 modes each)
├── instances.jsonl          # SWE-Agent task instances (6 tasks × 3 modes)
├── validate_suite.py        # Validation script
├── Makefile                 # Top-level build commands
└── PERF_SUITE_README.md     # This file

develop branch (upstream):
└── drivers/cpp/benchmarks/  # Original ParEval baseline implementations
```

## Task Details

| Task ID | File | Category | Bug Type | Complexity | Slowdown | Status |
|---------|------|----------|----------|------------|----------|--------|
| `perf_avg1` | `algorithms/reduce/average.hpp` | Reduction | Redundant reduce() in loop | O(n²) | ~45,000× | ✅ Working |
| `perf_ps1` | `algorithms/scan/prefix_sum.hpp` | Scan | Missing std::inclusive_scan | O(n²) | <1.5× | ⚠️ Optimized away at -O3 |
| `perf_contains1` | `algorithms/search/contains.hpp` | Search | No early return | O(n) | ~1000× | ✅ Working |
| `perf_relu1` | `algorithms/transform/relu.hpp` | Transform | Poor vectorization | O(n) | ~3× | ✅ Working |
| `perf_axpy1` | `algorithms/dense_la/axpy.hpp` | Dense LA | Function call overhead | O(n) | <1.5× | ⚠️ Too subtle at -O3 |
| `perf_gemm1` | `algorithms/dense_la/gemm.hpp` | Dense LA | Cache-unfriendly loop order | O(n³) | ~10× | ✅ Working |

**Note**: Some bugs (ps1, axpy1) are optimized away by aggressive compiler optimizations at -O3. These are included for educational purposes but may not show measurable slowdown in validation. For production use, consider compiling tests with -O2 or using the 4 working tasks.

## Prompt Modes

Each task has **3 problem statements** with different clarity levels:

1. **`problem_hinted.md`**: Gives exact file + line numbers of the bottleneck
   - Example: *"Optimize average in algorithms/reduce/average.cpp:15-20 - redundant reduce() calls..."*

2. **`problem_perf.md`**: Describes performance goal without location
   - Example: *"Make average faster - current performance is 5000x slower than expected..."*

3. **`problem_fix.md`**: Generic request with no hints
   - Example: *"Fix the performance issue in algorithms/reduce/average.cpp"*

## Using This Suite with SWE-Agent

### Prerequisites

```bash
# C++ compiler with C++17 support
g++ --version  # Should support C++17

# Google Test (for testing)
brew install googletest  # macOS
# OR
sudo apt-get install libgtest-dev  # Ubuntu

# Clone this repository
git clone https://github.com/KlaudiuszRydzy/ParEval.git
cd ParEval
git checkout perf-bugs-suite
```

### Quick Start

```bash
# 1. Validate the suite
python3 validate_suite.py

# 2. Build and run tests manually
cd tests
make all
./test_perf_avg1

# Run only correctness (should pass)
./test_perf_avg1 --gtest_filter="*.Correctness"

# Run only performance (should FAIL on perf-bugs-suite branch)
./test_perf_avg1 --gtest_filter="*.Performance"
```

### Running with SWE-Agent

```bash
# Run SWE-Agent with instances.jsonl
cd /path/to/SWE-agent
sweagent run-batch \
  --config config/default.yaml \
  --instances_path /path/to/ParEval/instances.jsonl \
  --agent.model.per_instance_cost_limit=0 \
  --agent.model.max_input_tokens=60000
```

## Performance Testing Methodology

Tests use **relative comparison** between buggy code (perf-bugs-suite branch) and correct baseline:

```cpp
TEST(Perf_Avg1, Performance) {
    // Measure current (buggy) implementation
    auto run_current = []() {
        volatile double result = average(test_data);
    };
    auto current_perf = measure_performance(run_current, n_runs=3, warmup=1);

    // Measure baseline (correct) implementation
    auto run_baseline = []() {
        volatile double result = average_baseline(test_data);
    };
    auto baseline_perf = measure_performance(run_baseline, n_runs=3, warmup=1);

    // Calculate slowdown factor
    double slowdown = current_perf.median / baseline_perf.median;

    // Pass if within 1.5x of baseline (buggy code will fail with 100-10000x slowdown)
    ASSERT_LT(slowdown, 1.5) << "Code is " << slowdown << "x slower than baseline";
}
```

## Validation

Run the validation script to confirm all tasks are properly configured:

```bash
python3 validate_suite.py
```

**Expected output on `perf-bugs-suite` branch:**
- ✓ All correctness tests PASS (bugs don't break functionality)
- ✓ All performance tests FAIL (bugs cause measurable slowdown)

### Adding a New Task

1. **Inject bug** into algorithm file with clear comments
2. **Create task directory**: `perf_tasks/perf_TASKID/`
3. **Write 3 problem statements**: `problem_hinted.md`, `problem_perf.md`, `problem_fix.md`
4. **Save baseline snippet**: `baseline_snippet.hpp` (original fast code from ParEval)
5. **Create test harness**: `tests/test_perf_TASKID.cpp`
6. **Add test to Makefile**: Update `TESTS` variable in `tests/Makefile`
7. **Add 3 JSONL entries** to `instances.jsonl` (one per mode)
8. **Update validation script**: Add test name to `PERF_TESTS` list
9. **Run validation**: `python3 validate_suite.py`

### Bug Injection Guidelines

**DO:**
- Use realistic patterns (redundant scans, missing STL algorithms, cache-unfriendly access)
- Preserve correctness (bugged code must produce correct output)
- Add clear comments explaining the bug
- Aim for 10-1000× slowdowns (measurable but fixable)
- Use actual performance patterns from ParEval baseline comparisons

**DON'T:**
- Use `sleep()` or artificial delays
- Break functionality (correctness tests must pass)
- Create bugs that are too subtle (<2× slowdown)
- Create bugs that require domain expertise beyond basic C++

## JSONL Schema

Each entry in `instances.jsonl` follows this schema:

```json
{
  "image_name": "gcc:13",
  "instance_id": "perf_avg1__hinted",
  "repo_name": "ParEval",
  "base_commit": "HEAD",
  "problem_statement": "Optimize average computation in...",
  "extra_fields": {
    "dataset_id": "pareval_cpp_perf",
    "task_id": "perf_avg1",
    "category": "reduce",
    "bug_type": "redundant_nested_scan",
    "entrypoint": "algorithms/reduce/average.hpp:average",
    "test_cmd": "cd tests && make test_perf_avg1 && ./test_perf_avg1",
    "inputs_desc": "Random vector n=50000",
    "detection_target": {
      "path": "algorithms/reduce/average.hpp",
      "line_span": [27, 32]
    },
    "prompt_mode": "hinted_exact_scope"
  }
}
```

## Key Differences from Python Performance Bug Suite

1. **Language**: C++ instead of Python
2. **Test Framework**: Google Test instead of pytest
3. **Build Step**: Requires compilation with `make` before running
4. **Timing Methodology**: More runs needed for statistical significance in C++
5. **Baseline Source**: Extracted from ParEval's verified sequential implementations
6. **Performance Bugs**: Focus on cache-efficiency, STL algorithm usage, loop ordering

---

Based on [ParEval: Can Large Language Models Write Parallel Code?](https://github.com/parallelcodefoundry/ParEval)
