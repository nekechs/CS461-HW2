#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <signal.h>

// #define ASSERT(X) if(!x) raise(SIGTRAP);
#define ASSERT(x) do {                                      \
    if(!x) {                                                \
        print_assertion_fail(__func__, __FILE__, __LINE__); \
        raise(SIGTRAP);                                     \
    }                                                       \
} while(0)

void print_assertion_fail(const char * function, const char * file, int num);

#endif