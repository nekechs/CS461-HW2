#include <stdio.h>
#include <string.h>
#include <math.h>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_vector.h>

#include "linear_regression.h"

void regmodel_reset_used(regression_model_t * model) {
    char * used_array = model->used_features;
    int num_elem = model->num_vars + 1;
    for(int i = 0; i < num_elem; i++) {
        used_array[i] = 1;
    }
}

// Assume that "model" points to a struct that already exists in memory.
void regmodel_init(regression_model_t * model, unsigned int num_vars) {
    model->num_vars = num_vars;

    model->used_features = (char *) malloc((1 + num_vars) * sizeof(char));    
    regmodel_reset_used(model);

    model->weights = (double *) malloc((1 + num_vars) * sizeof(double));
}

int regmodel_fit_minweight(regression_model_t * model,
                    arraylist_t * x_train, arraylist_t * y_train, double min_weight){
    regmodel_reset_used(model);
    regmodel_fit_inv(model, x_train, y_train);

    int used_count = 0;
    for(int i = 0; i < model->num_vars + 1; i++) {
        if(fabs(model->weights[i]) < min_weight) {
            model->used_features[i] = 0;
        } else {
            used_count++;
        }
    }

    regmodel_fit_inv(model, x_train, y_train);

    return used_count;
}

// DID NOT Have time to implement this.
// void regmodel_fit_sgd(regression_model_t * model,
//                     arraylist_t * x_train, arraylist_t * y_train,
//                     double learning_rate) {
    
// }

void regmodel_fit_inv(regression_model_t * model,
                    arraylist_t * x_train, arraylist_t * y_value) {
    gsl_matrix * X;
    
    const int num_vars = model->num_vars;                   // Basically the number of X variables
    const int num_datapoints = x_train->al_currentsize;     // The number of Y variables

    double * xvect_buffer = malloc((num_vars + 1) * sizeof(double));   // This buffer is to be used whenever
                                                                // accessing an arraylist of x variables
    xvect_buffer[0] = 1;
    
    // X matrix properties: n x m dimensions
    unsigned int n_x = num_datapoints;
    unsigned int m_x = 0;               // m_x is 1 + num_vars (that are IN USE!!)
    for(int i = 0; i < num_vars + 1; i++) {
        if(model->used_features[i]) {
            // We have encountered a feature that we use
            m_x++;
        }
    }

    if(!m_x) {
        // All parameters are not valid.... huh????
        // This means have failure case and use w_0 as the only weight.
        m_x = 1;
        model->used_features[0] = 1;
    }

    X = (gsl_matrix*) gsl_matrix_calloc(n_x, m_x);

    for(int i = 0; i < num_datapoints; i++) {
        al_get(x_train, i, xvect_buffer + 1);    // Access the i'th datapoint

        int j = 0;
        for(int weight_index = 0; weight_index < num_vars + 1; weight_index++) {
            if(model->used_features[weight_index]) {
                gsl_matrix_set(X, i, j, xvect_buffer[weight_index]);
                j++;
            }
        }

        // if(model->used_features[0]) gsl_matrix_set(X, i, 0, 1);         // The 0th vector is always 1.
        // int j = 1;
        // for(int xvar_index = 0; xvar_index < num_vars; xvar_index++) {
        //     if(model->used_features[xvar_index + 1]) {
        //         gsl_matrix_set(X, i, j, xvar_buffer[xvar_index]);
        //         j++;               
        //     }
        // }
    }
    free(xvect_buffer);

    gsl_matrix * XT = gsl_matrix_alloc(m_x, n_x);
    gsl_matrix_transpose_memcpy(XT, X);
    
    /* Now we compute X tranpose times X */
    gsl_matrix * XTX_tmp = gsl_matrix_alloc(m_x, m_x);
    gsl_matrix * XTX_inv = gsl_matrix_alloc(m_x, m_x);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, XT, X, 0.0, XTX_tmp);

    /* Onward to computing the inverse of XTX */
    int signum;
    gsl_permutation * p = gsl_permutation_alloc(m_x);
    gsl_linalg_LU_decomp(XTX_tmp, p, &signum);
    gsl_linalg_LU_invert(XTX_tmp, p, XTX_inv);

    gsl_permutation_free(p);
    gsl_matrix_free(XTX_tmp);

    /* Now we matrix multiply this by the transpose of X. */
    gsl_matrix * XTX_inv_XT = gsl_matrix_alloc(m_x, n_x);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, XTX_inv, XT, 0.0, XTX_inv_XT);

    /* Now we multiply this by y to get the weights */
    gsl_matrix_view Y = gsl_matrix_view_array(y_value->data, num_datapoints, 1);
    gsl_matrix * weights = gsl_matrix_alloc(m_x, 1);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, XTX_inv_XT, &Y.matrix, 0.0, weights);

    int weight_count = 0;
    for(int i = 0; i < num_vars + 1; i++) {
        if(model->used_features[i]) {
            model->weights[i] = gsl_matrix_get(weights, weight_count, 0);
            weight_count++;
        } else {
            model->weights[i] = 0;
        }
    }

    gsl_matrix_free(weights);
    gsl_matrix_free(XTX_inv_XT);
    gsl_matrix_free(XTX_inv);
    gsl_matrix_free(XT);
    gsl_matrix_free(X);
    // free(xvect_buffer);
}

void regmodel_destroy(regression_model_t * model) {
    free(model->used_features);
    free(model->weights);

    model->num_vars = 0;
}

double regmodel_apply(regression_model_t * model, double * input_vector, unsigned int num_elem) {
    double total = model->weights[0];
    for(int i = 0; i < num_elem; i++) {
        total += model->weights[i + 1] * input_vector[i];
    }

    return total;
}

double regmodel_get_error(regression_model_t * model, arraylist_t * x_test, arraylist_t * y_test) {
    double total_err = 0;

    double * buf = malloc(x_test->al_elemsize);
    for(int i = 0; i < x_test->al_currentsize; i++) {
        al_get(x_test, i, buf);
        double actual_y;
        // printf("Actual: %lf; ", actual_y);
        al_get(y_test, i, &actual_y);
        double model_y = regmodel_apply(model, buf, x_test->al_elemsize / sizeof(double));
        // printf("MOdel y: %lf\n", model_y);

        double dev = actual_y - model_y;
        total_err += dev * dev;
    }
    free(buf);

    return total_err / x_test->al_currentsize;
}

void print_matrix(gsl_matrix * matrix) {
    int n = matrix->size1;
    int m = matrix->size2;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%lf\t\t", gsl_matrix_get(matrix, i, j));
        }
        printf("\n");
    }
}