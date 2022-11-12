#ifndef _RANDOM_H_
#define _RANDOM_H_

#define NORMAL_STDEV(mu, sigma) (stdnormal_generator() * sigma + mu)

/**
 * @brief Set the seed for the random number generator
 * 
 * @param seed 
 */
void set_seed(int seed);

/**
 * @brief Generates a random double between 0 (INCLUSIVE) and 1 (NOT INCLUSIVE). 
 * 
 * @return double - a random double from 0 to 1. 
 */
double rand_fp();

int bernoulli_generator(double probability);

double stdnormal_generator();

double normal_generator(double mu, double var);
#endif