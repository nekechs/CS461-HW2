#ifndef _STAT_H_
#define _STAT_H_

#include "arraylist.h"

/**
 * @brief Assuming an arraylist of solely double vectors, calculate the average value of one index of the vector across all of the vectors.
 * 
 * @param datapoints The arraylist of vectors
 * @param index The specific index for the vector element you want to compute the average of
 * @return double is the actual computed value for the average
 */
double dpoint_avg_of_index(arraylist_t * datapoints, int index);

/**
 * @brief Assuming an arraylist of vectors, calculate the sample variance of one vector element across all of the vectors. 
 * 
 * @param datapoints The arraylist of vectors
 * @param index The specific index for the vector element that you want to compute the variance of
 * @return double is the actual computed value for the variance estimate (sample variance)
 */
double dpoint_var_of_index(arraylist_t * datapoints, int index);

/**
 * @brief Assuming an arraylist of vectors (composed of doubles), calculate the corr. coeff. of two vector indices across all vectors. 
 * 
 * @param datapoints The arraylist of (double) vectors
 * @param var1_ind The first variable's index in the vector
 * @param var2_ind The second variable's index in the vector
 * @return double The correlation of the two variables
 */
double dpoint_corr_of_indices(arraylist_t * datapoints, int var1_ind, int var2_ind);

/**
 * @brief Based on a given list of datapoints (INCLUDES the x vector and the y value for each datapoint vector)
 * calculates the information gain with respect to the provided variable var for all variables, and returns
 * the highest information gain (stores the index in *var) 
 * 
 * @param datapoints 
 * @param num_vars 
 * @param var 
 * @return double Information Gainedd
 */
double dpoint_get_max_ig(arraylist_t * datapoints, int num_vars, int * var);

double constant_get_error(double constant, arraylist_t * x_test, arraylist_t * y_test);

#endif