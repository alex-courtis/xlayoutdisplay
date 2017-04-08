#ifndef XLAYOUTDISPLAYS_MACROS_H
#define XLAYOUTDISPLAYS_MACROS_H

#include <stdlib.h>
#include <stdio.h>

// TODO: refactor this as a throw from cpp, with c functions returning zeros instead of FAIL
#define FAIL(...) { fprintf(stderr, "FAIL: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

#endif //XLAYOUTDISPLAYS_MACROS_H
