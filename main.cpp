#include <cstdio>
#include <cstdlib>
#include <exception>

#include "src/Layout.h"

using namespace std;

int main(int argc, char **argv) {
    try {
        layout(argc, argv);
        return WEXITSTATUS(layout(argc, argv));
    } catch (const exception &e) {
        fprintf(stderr, "FAIL: %s, exiting\n", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        fprintf(stderr, "EPIC FAIL: unknown exception, exiting\n");
        return EXIT_FAILURE;
    }
}
