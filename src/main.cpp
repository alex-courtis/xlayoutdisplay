#include "Layout.h"

using namespace std;

int main(int argc, char **argv) {
    try {
        return (Layout(argc, argv).apply());
    } catch (const exception &e) {
        fprintf(stderr, "FAIL: %s, exiting\n", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        fprintf(stderr, "EPIC FAIL: unknown exception, exiting\n");
        return EXIT_FAILURE;
    }
}
