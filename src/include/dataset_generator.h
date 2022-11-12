#ifndef _DATASET_GENERATOR_H_
#define _DATASET_GENERATOR_H_

#define HW2_X_NUM 15

#include "arraylist.h"

// Based on a number of samples, generates a new arraylist by inserting n number of samples into a new arraylist. Returns a pointer to the arraylist.
// This function does actually allocate a new arraylist, so make sure to use al_destroy() on the arraylist. However, make sure that the struct itself
// is allocated somewhere else. MAKE SURE x_vals IS NOT INITIALIZED!!!!
void gen_hw2_xvals(arraylist_t * x_vals, int num_samples);

// Takes an arraylist of x values, and generates an arraylist by computing the Y value on each vector and storing those new y values in a new arraylist.
// x_vals is not going to be modified, but y_vals is actually subject to change. MAKE SURE y_vals IS NOT INITIALIZED!!!!
void gen_hw2_yvals(arraylist_t * x_vals, double sigma, arraylist_t * y_vals);

#endif
