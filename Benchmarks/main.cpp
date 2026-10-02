/************************************/
/*           main.cpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "../Core/Testing/Benchmarker.hpp"

// The benchmark groups of the workspace, one function per benchmark file. A group registers
// its benchmarks when the run reaches it, which is what lets the command line decide what is
// measured before anything is: the whole file has to be named here and nowhere else.
void bench_u8(Benchmarker &p_benchmarker);

BENCHMARK_GROUP(ratlab, bench_u8)
BENCHMARK_MAIN(ratlab)