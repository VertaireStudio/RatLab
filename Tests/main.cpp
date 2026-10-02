/************************************/
/*           main.cpp               */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "../Core/Testing/Tester.hpp"

// The test cases register themselves at static-initialization time, from every translation
// unit which includes 'Tester.hpp'. Adding a new test file to the target is all it takes.
// Returns 0 when every executed check passed, otherwise 1, which makes the process
// report a failure to CMake, CTest and every other CI runner.
int main(const int p_argc, char **p_argv) {
    Tester tester;
    return tester.run(p_argc, p_argv);
}
