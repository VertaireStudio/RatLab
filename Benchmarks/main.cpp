/************************************/
/*           main.cpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "../Core/Testing/Benchmarker.hpp"

// The benchmarks register themselves at static-initialization time, from every translation
// unit which includes 'Benchmarker.hpp'. Adding a new benchmark file to the target is all it
// takes. The amount of iterations is calibrated per benchmark at runtime, so the reported
// timings stay comparable between machines.
int main(const int p_argc, char **p_argv) {
    Benchmarker benchmarker;
    return benchmarker.run(p_argc, p_argv);
}
