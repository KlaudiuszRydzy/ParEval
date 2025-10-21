Optimize gemm in algorithms/dense_la/gemm.hpp:30-36 - ijk loop order causes cache-unfriendly access to matrix B, reorder to ikj for better locality
