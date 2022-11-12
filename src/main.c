#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "math_lib.h"
#include "random.h"

#define NUM_TRIALS 100

int compare_double(const void * d1, const void * d2) {
    double num1 = *(double*)d1;
    double num2 = *(double*)d2;

    // printf("double 1: %lf; double 2: %lf\n", num1, num2);

    if(num1 < num2) return -1;
    if(num1 > num2) return 1;
    return 0;
}

int main(int argc, char ** argv) {
    printf("bruh\n");
    // printf("%d is the inv sqrt of 2.\n", tester());

    srand(time(NULL));

    double var = 0;
    double mean = 0;

    double data[NUM_TRIALS];

    for(int i = 0; i < NUM_TRIALS; i++) {
        double rand = NORMAL_STDEV(100, 3);
        mean += rand;
        data[i] = rand;
        // printf("%lf\n", rand);
    }

    mean = mean / NUM_TRIALS;

    qsort(data, NUM_TRIALS, sizeof(double), &compare_double);

    for(int i = 0; i < NUM_TRIALS; i++) {
        var += (mean - data[i]) * (mean - data[i]);
        printf("%lf\n", data[i]);
    }

    var /= NUM_TRIALS;

    printf("Variance: %lf\n", var);
}