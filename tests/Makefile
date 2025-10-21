# Makefile for performance bug tests
# Adapted from ParEval's build system

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
INCLUDES = -I.. -I../perf_tasks/_templates

# Google Test flags (installed via Homebrew)
GTEST_PREFIX = /opt/homebrew/opt/googletest
GTEST_INCLUDES = -I$(GTEST_PREFIX)/include
GTEST_LIBS = -L$(GTEST_PREFIX)/lib -lgtest -lgtest_main -pthread

# Test targets
TESTS = test_perf_avg1 test_perf_ps1 test_perf_contains1 test_perf_relu1 test_perf_axpy1 test_perf_gemm1

# Default target
all: $(TESTS)

# Generic rule for building tests
test_%: test_%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(GTEST_INCLUDES) $< $(GTEST_LIBS) -o $@

# Run all tests
test: $(TESTS)
	@for test in $(TESTS); do \
		echo "Running $$test..."; \
		./$$test || exit 1; \
	done

# Run only correctness tests
test-correctness: $(TESTS)
	@for test in $(TESTS); do \
		echo "Running correctness test in $$test..."; \
		./$$test --gtest_filter="*.Correctness" || exit 1; \
	done

# Run only performance tests (should FAIL on bugged branch)
test-performance: $(TESTS)
	@for test in $(TESTS); do \
		echo "Running performance test in $$test..."; \
		./$$test --gtest_filter="*.Performance" || exit 1; \
	done

# Clean
clean:
	rm -f $(TESTS) *.o

.PHONY: all test test-correctness test-performance clean
