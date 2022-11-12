#include <stdio.h>
#include <math.h>
#include <float.h>

#include "stat.h"

double dpoint_avg_of_index(arraylist_t * datapoints, int index) {
    double running_average = 0;

    double * buffer = malloc(datapoints->al_elemsize);
    for(int i = 0; i < datapoints->al_currentsize; i++) {
        al_get(datapoints, i, buffer);

        double current_number = buffer[index];
        
        double avg_frac = i / (i + 1.0);
        double current_frac = 1 / (i + 1.0);

        running_average = running_average * avg_frac + current_number * current_frac;

        // printf("Val at iteration %d: %lf\n", i, running_average);
    }
    free(buffer);

    return running_average;
}

double dpoint_var_of_index(arraylist_t * datapoints, int index) {
    // Size of the arraylist
    const int num_elem = datapoints->al_currentsize;
    if(num_elem <= 1) return 0;

    double avg = dpoint_avg_of_index(datapoints, index);
    // Running total of the sample variance
    double var = 0;

    double * buffer = malloc(datapoints->al_elemsize);
    for(int i = 0; i < num_elem; i++) {
        al_get(datapoints, i, buffer);

        double temp_val = buffer[index];
        temp_val = temp_val - avg;
        // printf("Val @ ind %d: %lf. Diff: %lf\n", i, buffer[index],temp_val);
        temp_val = temp_val * temp_val;
        var += temp_val;
    }
    free(buffer);

    return var / (num_elem - 1);
}

double dpoint_corr_of_indices(arraylist_t * datapoints, int var1_ind, int var2_ind) {
    const int num_elem = datapoints->al_currentsize;
    if(num_elem  <= 1) return 0;

    double avg_var1 = dpoint_avg_of_index(datapoints, var1_ind);
    double avg_var2 = dpoint_avg_of_index(datapoints, var2_ind);

    double total_error_var1 = 0;    // Sum of the square differences in all var1 vs var1 average (related to var1 variance)
    double total_error_var2 = 0;    // Sum of the square differences in all var2 vs var2 average (related to var2 variance)
    double total_co_error = 0;      // Sum of the products of each var1 deviation and var2 deviation (related to var1 & var2 covariance)

    double * buffer = malloc(datapoints->al_elemsize);
    for(int i = 0; i < num_elem; i++) {
        al_get(datapoints, i, buffer);

        double var1_current = buffer[var1_ind];
        double var2_current = buffer[var2_ind];

        double var1_current_dev = var1_current - avg_var1;
        double var2_current_dev = var2_current - avg_var2;

        total_error_var1 += var1_current * var1_current;
        total_error_var2 += var2_current * var2_current;
        total_co_error += var1_current * var2_current;
    }
    free(buffer);

    double denominator = total_error_var1 * total_error_var2;

    if(denominator == 0) {
        return 0;
    }

    return (total_co_error) / sqrt(denominator);
}

double compute_entropy_given_x(arraylist_t * datapoints, int num_vars, int var) {
    const size_t num_points = datapoints->al_currentsize;

    /* Store the number of occurences of each x class*/
    int x_hmap[2] = {0};

    /* For a given x class (row), store number of occurences of each y class*/
    int y_hmap[2][3] = {0};

    double * buffer = malloc(datapoints->al_elemsize);
    for(int i = 0; i < num_points; i++) {
        al_get(datapoints, i, buffer);

        double x_val = buffer[var];
        double y_val = buffer[num_vars];

        if(x_val < 0) {
            /* Here, we say X = -1 */
            x_hmap[0]++;

            if(y_val > 0) y_hmap[0][2]++; 
            else if(y_val < 0) y_hmap[0][0]++;
            else y_hmap[0][1]++;
        } else {
            /* Here, we say X = 1 */
            x_hmap[1]++;

            if(y_val > 0) y_hmap[1][2]++; 
            else if(y_val < 0) y_hmap[1][0]++;
            else y_hmap[1][1]++;
        }
    }
    free(buffer);

    double ent_y_given_x = 0;
    for(int x_class = 0; x_class < 2; x_class++) {
        double ent_y_given_xclass = 0;
        for(int y_class = 0; y_class < 3; y_class++) {
            /* Essentially, here we are calculating the probability that Y is y given X is x */
            double p_y_x = ((double)y_hmap[x_class][y_class])/x_hmap[x_class];
            ent_y_given_xclass -= p_y_x > 0 ? p_y_x * log2(p_y_x) : 0;
        }

        /* calculate the probability that X is equal to x */
        double p_x = ((double)x_hmap[x_class]) / num_vars;

        ent_y_given_x += p_x * ent_y_given_xclass;
    }

    return ent_y_given_x;
}

double dpoint_get_max_ig(arraylist_t * datapoints, int num_vars, int * var_to_split) {
    double ent_y = 0;
    double ent_y_given_x = DBL_MAX;

    const size_t num_points = datapoints->al_currentsize;

    /* Here, we are storing the number of occurences of each Y class*/
    int y_hmap[3] = {0};
    double * buffer = malloc(datapoints->al_elemsize);
    for(int i = 0; i < num_points; i++) {
        al_get(datapoints, i, buffer);
        double y_val = buffer[num_vars];
        if(y_val > 0) y_hmap[2]++;
        else if(y_val < 0) y_hmap[0]++;
        else y_hmap[1]++;
    }
    free(buffer);

    /* Calculate the entropy of Y */
    for(int i = 0; i < 3; i++) {
        double p_y = ((double)y_hmap[i]) / num_points;
        ent_y -= (p_y == 0 ? 0 : p_y * log2(p_y));
    }
    
    for(int var = 0; var < num_vars; var++) {
        double candidate = compute_entropy_given_x(datapoints, num_vars, var);
        // printf("Information gain for variable %d: %lf\n", var, ent_y - candidate);
        if(candidate < ent_y_given_x) {
            ent_y_given_x = candidate;
            *var_to_split = var;
        }
    }

    return ent_y - ent_y_given_x;
}

double constant_get_error(double constant, arraylist_t * x_test, arraylist_t * y_test) {
    double total_err = 0;

    double * buf = malloc(x_test->al_elemsize);
    for(int i = 0; i < x_test->al_currentsize; i++) {
        al_get(x_test, i, buf);
        double actual_y;

        double dev = actual_y - constant;
        total_err += dev * dev;
    }
    free(buf);

    return total_err / x_test->al_currentsize;
}