cmake_minimum_required(VERSION 3.14)
project(ParEval_PerfBugSuite CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags (O2 to preserve performance bugs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

# Find Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/perf_tasks/_templates)

# Function to add a performance test
function(add_perf_test test_name)
    add_executable(${test_name} tests/${test_name}.cpp)
    target_link_libraries(${test_name} GTest::gtest_main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Add all performance tests
add_perf_test(test_perf_avg1)
# add_perf_test(test_perf_ps1)
# add_perf_test(test_perf_contains1)
# add_perf_test(test_perf_relu1)
# add_perf_test(test_perf_axpy1)
# add_perf_test(test_perf_gemm1)

# Custom target to run all tests
add_custom_target(run_tests
    COMMAND ctest --output-on-failure
    DEPENDS test_perf_avg1
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
