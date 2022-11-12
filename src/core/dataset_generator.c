#include "dataset_generator.h"
#include "random.h"

void create_xvect(double * buffer) {
    for(int i = 0; i < HW2_X_NUM; i++) {
        buffer[i] = bernoulli_generator(0.5) ? -1 : 1;
    }
}

double default_y(double * buffer, double sigma) {
    double result = 0.9 * (
        buffer[0] + 0.9 * (
        buffer[1] + 0.9 * (
        buffer[2] + 0.9 * (
        buffer[3] + 0.9 * (
        buffer[4]
        )
        )
        )
        )
    ) + NORMAL_STDEV(0, sigma);

    if(result < 0) return -1;
    if(result > 0) return 1;
    return 0;
}

void gen_hw2_xvals(arraylist_t * x_vals, int num_samples) {
    al_init(x_vals, HW2_X_NUM * sizeof(double));    // Notice that we are treating each x value as a vector of doubles.
    
    double * xvect_buf = malloc(HW2_X_NUM * sizeof(double));
    for(int i = 0; i < num_samples; i++) {
        create_xvect(xvect_buf);
        al_insert(x_vals, i, xvect_buf);
    }
    free(xvect_buf);
}

void gen_hw2_yvals(arraylist_t * x_vals, double sigma, arraylist_t * y_vals) {
    al_init(y_vals, sizeof(double));                // This is just an arraylist of doubles.
    unsigned int num_samples = x_vals->al_currentsize;

    double y_val = 0;
    double * x_vect = malloc(HW2_X_NUM * sizeof(double));
    for(int i = 0; i < num_samples; i++) {
        al_get(x_vals, i, x_vect);
        y_val = default_y(x_vect, sigma);
        al_insert(y_vals, i, &y_val);
    }
    free(x_vect);
}
