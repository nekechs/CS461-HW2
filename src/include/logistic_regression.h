#ifndef _LOGISTIC_REGRESSION_H_
#define _LOGISTIC_REGRESSION_H_

#include "arraylist.h"

typedef struct {
    /* Stores the number of variables (not including the bias term, X0 = 1) */
    unsigned int num_vars;

    /* Stores all of the weights (including the X0 = 1 term)*
    /* Contains space for (num_vars + 1) doubles. */
    double * weights;
} logistic_model_t;

/* Computes the logistic function for a given x value */
double logistic(double x);

/* Computes the error on the model with respect to all of the data. */
double lgst_model_get_error(logistic_model_t * model, arraylist_t * x_test, arraylist_t * y_test);

/* Initializes a (not previously initialized) logistic model. Basically sets num_vars and allocates enough mem for the weights */
void lgst_model_init(logistic_model_t * model, unsigned int num_vars);

/* Takes a bunch of data and fits the model to the data by performing stochastic gradient descent.*/
void lgst_model_fit_sgd(logistic_model_t * model, 
                arraylist_t * x_train, arraylist_t * y_train);

/* Resets num_vars to 0 and frees the memory for the weights */
void lgst_model_destroy(logistic_model_t * model);

/* Based on inputs to a given logistic model, this gives the estimated value that occurs at certain X values */
double lgst_model_apply(logistic_model_t * model, double * input_vector, unsigned int num_elem);

#endif