#include <math.h>
#include <stdlib.h>

#include "random.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double rand_fp() {
    int rand_int = rand() - 1;

    if(rand_int < 0) {
        rand_int = 0;
    }
    
    return (double)rand_int / RAND_MAX;
}

double stdnormal_generator() {
    // Here we are using the Box-Muller method of generating a normally distributed random variable
    double uniform_1 = rand_fp();
    double uniform_2 = rand_fp();

    double angle = 2.0 * M_PI * uniform_2;
    double sqrt_val = sqrt(-2.0 * log(uniform_1));

    return sqrt_val * sin(angle);
}

double normal_generator(double mu, double var) {
    return NORMAL_STDEV(mu, sqrt(var));
}

int bernoulli_generator(double probability) {
    return rand_fp() < probability;
}