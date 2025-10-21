Optimize relu in algorithms/transform/relu.hpp:27-33 - manual loop with branch prevents vectorization, use std::transform with std::max instead
