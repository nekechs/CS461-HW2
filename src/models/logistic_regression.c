#include "logistic_regression.h"
#include "arraylist.h"

double logistic(double x) {
    return 1 / (1 + exp(-x));
}

double lgst_model_get_error(logistic_model_t * model, arraylist_t * x_test, arraylist_t * y_test) {
    
}

void lgst_model_init(logistic_model_t * model, unsigned int num_vars) {
    model->num_vars = num_vars;
    model->weights = (double *) malloc(sizeof(double) * (num_vars + 1)); 
}

void lgst_model_fit_sgd(logistic_model_t * model, 
                arraylist_t * x_train, arraylist_t * y_train) {
    
}

void lgst_model_destroy(logistic_model_t * model) {
    model->num_vars = 0;
    free(model->weights);
}

double lgst_model_apply(logistic_model_t * model, double * input_vector, unsigned int num_elem) {
    double result = 0;
}
