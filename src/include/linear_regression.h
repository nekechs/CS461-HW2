#ifndef _LINEAR_REGRESSION_H_
#define _LINEAR_REGRESSION_H_

#include "arraylist.h"

#define SGD_BATCHSIZE 10

enum REG_METHOD {
    UNKNOWN = 0,
    REG_SGD,
    REG_INV
};

typedef struct {
    unsigned int num_vars;
    char * used_features;               // Vector with length num_vars. IF the i th element is 0, the i th feature is considered for
                                        // regression. Otherwise, it is ignored.
    double * weights;                   // Will have (num_vars + 1) number of doubles stored here.
    enum REG_METHOD regression_type;    // SPecifies the type of regression (sgd vs mat.)
} regression_model_t;

// Initializes a model based on a given number of variables to be used in that model.
void regmodel_init(regression_model_t * model, unsigned int num_vars);

// Fits a model to a given list of training inputs and training outputs.
// MUST INITIALIZE the model first using reg_init before calling this function.
void regmodel_fit_inv(regression_model_t * model,
                    arraylist_t * x_train, arraylist_t * y_train);

// Does the same thing as regmodel_fit(), except this makes sure that all the weights are at least a certain value.
// Otherwise, the model is re-fitted with the lowest weight of the model essentially removed.
// Also this returns the number of features that make it into the final model.
int regmodel_fit_minweight(regression_model_t * model,
                    arraylist_t * x_train, arraylist_t * y_train, double min_weight);

void regmodel_destroy(regression_model_t * model);

double regmodel_apply(regression_model_t * model, double * input_vector, unsigned int num_elem);

double regmodel_get_error(regression_model_t * model, arraylist_t * x_test, arraylist_t * y_test);

#endif