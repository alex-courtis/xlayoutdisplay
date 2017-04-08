#ifndef XLAYOUTDISPLAYS_MACROS_H
#define XLAYOUTDISPLAYS_MACROS_H

#define FAIL(...) { fprintf(stderr, "FAIL: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

#endif //XLAYOUTDISPLAYS_MACROS_H
