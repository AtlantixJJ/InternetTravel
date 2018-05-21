#include "extern.h"

void test_python() {
    printf("Hello from C++\n");
}

int set_argument(char *s) {
    memcpy(s, "here", 4);
}