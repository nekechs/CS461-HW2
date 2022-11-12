#include <stdio.h>
#include <stdlib.h>

#include "assert.h"

void print_assertion_fail(const char * function, const char * file, int num) {
    printf("Assertion failed in %s; line %d in %s\n", function, num, file);
}