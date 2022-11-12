#include <stdio.h>
#include <limits.h>
#include <float.h>

#include "decisiontree.h"
#include "arraylist.h"
#include "stat.h"

#ifndef DEBUG
#define DEBUG 0
#endif

// Takes a pointer to a node reference and then recursively deletes that node
// and all nodes below it.
void dtreenode_delete(dtreenode_t * head);

int var_compared = 0; // Do not let this variable get to at least the "d" number
int compare_datapoint(void * dpoint1, void * dpoint2) {
    double x_d1 = *( ((double *)dpoint1 ) + var_compared );
    double x_d2 = *( ((double *)dpoint2 ) + var_compared );

    if(x_d1 < x_d2) return -1;
    if(x_d1 > x_d2) return 1;
    return 0;
}

// Recursive function that takes some node in the decision tree, the training data relevant to that node, and then 
// gives the node left and right children that represent optimal decision tree splits.
void split_under_node(dtreenode_t * head, arraylist_t * relevant_datapoints, unsigned int num_var,
                        dtree_t * orig_tree);

/**
 * @brief Based on a list of data points, find the optimal threshold for splitting the data based on a given variable. 
 * 
 * @param datapoints list of datapoint vectors that are to be splitted 
 * @param var_to_split the variable (index starting at 0) to be split on (decide beforehand what this is)
 * @param num_vars number of X variables present for this data (NOT the total number of elements in this
 *                  vector---that would be num_vars + 1, because then you would have to include the Y variable)
 * @return double is the optimal threshold on which splitting would give the least error
 */
double find_optimal_threshold(arraylist_t * datapoints, unsigned int var_to_split, unsigned int num_vars);

void dtree_fit(dtree_t * tree, int num_vars,
                arraylist_t * train_x, arraylist_t * train_y, 
                arraylist_t * validation_x, arraylist_t * validation_y) {
    // TODO: Finish this
    if(train_x->al_currentsize != train_y->al_currentsize) {
        // THere has been an error.
        return;
    }

    if(tree->trucate_mode == UNKNOWN) {
        dtree_setmode_depth(tree, DEFAULT_MAX_DEPTH);
    }

    //  Arraylist of bunch of doubles of x (one for each different X-value) and the corresponding y value
    arraylist_t datapoints;
    al_init(&datapoints, sizeof(double) * (num_vars + 1));

    double * datapoint_buffer = malloc(sizeof(double) * (num_vars + 1));
    for(int i = 0; i < train_x->al_currentsize; i++) {
        al_get(train_x, i, datapoint_buffer);
        al_get(train_y, i, datapoint_buffer + num_vars);
        al_insert(&datapoints, datapoints.al_currentsize, datapoint_buffer);
    }
    free(datapoint_buffer);

    dtreenode_t * head = malloc(sizeof(dtreenode_t));
    head->parent = NULL;
    head->left = NULL;
    head->right = NULL;
    head->depth = 0;
    
    split_under_node(head, &datapoints, num_vars, tree);
    tree->head_ptr = head;
    
    al_destroy(&datapoints);
}

void split_under_node(dtreenode_t * head, arraylist_t * relevant_datapoints, unsigned int num_vars, 
                        dtree_t * orig_tree) {
    double y_avg = dpoint_avg_of_index(relevant_datapoints, num_vars);
    head->average = y_avg;
    head->threshold = DBL_MIN;
    head->var = -1;

    // Now take into account stopping condition.
    if(orig_tree->trucate_mode == TRUNC_BY_DEPTH && head->depth > orig_tree->depth) {
        // puts("bruh");
        return;
    }

    // printf("A node's depth: %d; Current size of node: %d\n", head->depth, relevant_datapoints->al_currentsize);

    switch(orig_tree->trucate_mode) {
        case TRUNC_BY_DEPTH:
            if(head->depth > orig_tree->depth) {
                return;
            }
            break;
        case TRUNC_BY_SAMPLE_SIZE:
            if(relevant_datapoints->al_currentsize < orig_tree->sample_size) {
                return;
            }
            break;
        default:
            orig_tree->trucate_mode = TRUNC_BY_DEPTH;
            orig_tree->depth = DEFAULT_MAX_DEPTH;
            if(head->depth > orig_tree->depth) {
                return;
            }
    }

    double biggest_corr = -1;
    int biggest_corr_var = -1;  // We want to split on the biggest (greatest magnitude)
                                // correlated variable. This int should have the highest corr. 
    for(int i = 0; i < num_vars; i++) {
        double corr = dpoint_corr_of_indices(relevant_datapoints, i, num_vars);
        double abs_corr = fabs(corr);
        if(abs_corr > biggest_corr) {
            biggest_corr = abs_corr;
            biggest_corr_var = i;
        }
        // printf("Correlation with variable %d: %lf\n", i, corr);
    }

    head->var = biggest_corr_var;
    head->threshold = find_optimal_threshold(relevant_datapoints, biggest_corr_var, num_vars);

    if(head->threshold == DBL_MIN) {
        // The data we want to split on somehow is of size 1. When this happens, just truncate it here no questions asked
        // despite the fact that this is actually depth based.
        return;
    }

    arraylist_t left_data;
    arraylist_t right_data;

    al_init(&left_data, relevant_datapoints->al_elemsize);
    al_init(&right_data, relevant_datapoints->al_elemsize);

    const size_t num_datapoints = relevant_datapoints->al_currentsize;
    double * datapoint_buffer = malloc(relevant_datapoints->al_elemsize);
    int tiebreaker = 0;
    for(int i = 0; i < num_datapoints; i++) {
        al_get(relevant_datapoints, i, datapoint_buffer);
        
        double xval = datapoint_buffer[biggest_corr_var];
        if(xval < head->threshold) {
            al_insert(&left_data, left_data.al_currentsize, datapoint_buffer);
        } else if (xval > head->threshold) {
            al_insert(&right_data, right_data.al_currentsize, datapoint_buffer);
        } else {
            arraylist_t * picked_data = tiebreaker % 2 ? &left_data : &right_data;
            al_insert(picked_data, picked_data->al_currentsize, datapoint_buffer);
            tiebreaker++;
        }
    }
    free(datapoint_buffer);

    // print_double_vect_al(relevant_datapoints);
    // printf("Left array size: %d; Right array size: %d; Trheshold: %lf; Var to split: %d\n\n", left_data.al_currentsize, right_data.al_currentsize, head->threshold, head->var);

    dtreenode_t * left_head = malloc(sizeof(dtreenode_t));
    left_head->depth = head->depth + 1;
    left_head->parent = head;
    left_head->left = NULL;
    left_head->right = NULL;
    split_under_node(left_head, &left_data, num_vars, orig_tree);
    head->left = left_head;

    dtreenode_t * right_head = malloc(sizeof(dtreenode_t));
    right_head->depth = head->depth + 1;
    right_head->parent = head;
    right_head->left = NULL;
    right_head->right = NULL;
    split_under_node(right_head, &right_data, num_vars, orig_tree);
    head->right = right_head;
    

    al_destroy(&left_data);
    al_destroy(&right_data);

    return;
}

double find_optimal_threshold(arraylist_t * datapoints, unsigned int var_to_split, unsigned int num_vars) {
    double optimal_threshold = FLT_MIN;

    var_compared = var_to_split;
    qsort(datapoints->data, datapoints->al_currentsize, datapoints->al_elemsize, compare_datapoint);

    const int old_list_size = datapoints->al_currentsize;   // The number of datapoints in the original datapoints list of vectors

    // We make a list of datapoints with no duplication (has only relevant x variable and y variable). Means take average of dup. elements.
    arraylist_t datapoints_no_dup;
    al_init(&datapoints_no_dup, 2 * sizeof(double));
    double * no_dup_buffer = malloc(2 * sizeof(double));    // Used as a buffer for the non-duplicated list of values
    double * old_buffer = malloc(datapoints->al_elemsize);  // Used as a buffer for the original, raw list of values


    double previous_xval = FLT_MIN;
    double dup_ysum = 0;                // Keeps track of the sum of y values in the case of duplicated x values
    int _dup_count = 0;                 // Keeps track of how many successive duplicated x values grouped together
    int current_raw_elem = 0;
    while(current_raw_elem < datapoints->al_currentsize) {
        al_get(datapoints, current_raw_elem, old_buffer);
        double xval = old_buffer[var_to_split];     // Access the relevant x value (x_n where n is the variable being analyzed)
        double yval = old_buffer[num_vars];         // Access the y value


        if(xval == previous_xval) {
            // We have found a duplicate value!!
            dup_ysum += yval;
            _dup_count += 1;
        } else {
            // We could have come out of a duplicate set, and as such, need to add the average of the duplicates here
            if(dup_ysum) {
                no_dup_buffer[0] = previous_xval;
                no_dup_buffer[1] = dup_ysum / _dup_count;
                al_insert(&datapoints_no_dup, datapoints_no_dup.al_currentsize, no_dup_buffer);
            }
            no_dup_buffer[0] = xval;
            no_dup_buffer[1] = yval;

            al_insert(&datapoints_no_dup, datapoints_no_dup.al_currentsize, no_dup_buffer);

            dup_ysum = 0;
            _dup_count = 0;
        }

        previous_xval = xval;
        current_raw_elem++;
    }
    
    if(dup_ysum) {
        no_dup_buffer[0] = previous_xval;
        no_dup_buffer[1] = dup_ysum / _dup_count;
        al_insert(&datapoints_no_dup, datapoints_no_dup.al_currentsize, no_dup_buffer);
    }

    // Now we have the set of datapoints without duplicate elements.
    const int no_dup_count = datapoints_no_dup.al_currentsize;// Size of the no-duplicate array

    // print_double_vect_al(&datapoints_no_dup);

    double lowest_error = DBL_MAX;      // Keeps track of the lowest encountered error
    int lowest_error_lindex = -1;       // Supposed to be the index located to the left of the most optimal threshold 
                                        // (-1 for an optimal threshold below all of the xvals)

    // Test a threshold below all of the xvals
    for(int left_side = 0; left_side < no_dup_count - 1; left_side++) {
        // Test the midpoints in between
        double left_y_avg = 0;
        double right_y_avg = 0;

        for(int i = 0; i <= left_side; i++) {
            al_get(&datapoints_no_dup, i, no_dup_buffer);
            
            double current_yval = no_dup_buffer[1];

            double running_avg_frac = i / (i + 1.0);
            double current_frac = 1 / (i + 1.0);

            left_y_avg = running_avg_frac * left_y_avg + current_frac * current_yval;
        }

        unsigned int current_right_count = 0;
        for(int i = left_side + 1; i < no_dup_count; i++) {
            al_get(&datapoints_no_dup, i, no_dup_buffer);
            double current_yval = no_dup_buffer[1];

            double running_avg_frac = current_right_count / (current_right_count + 1.0);
            double current_frac = 1 / (current_right_count + 1.0);

            right_y_avg = running_avg_frac * right_y_avg + current_frac * current_yval;
            current_right_count++;
        }

        double error_left = 0;
        double error_right = 0;

        // Copute left error.
        int current_left_count = 0;
        for(int i = 0; i <= left_side; i++) {
            al_get(&datapoints_no_dup, i, no_dup_buffer);
            double deviation = no_dup_buffer[1] - left_y_avg;
            double sq_deviation = deviation * deviation;

            double running_err_frac = current_left_count / (current_left_count + 1.0);
            double current_sqdev_frac = 1 / (current_left_count + 1.0);
            
            error_left = running_err_frac * error_left + current_sqdev_frac * sq_deviation;

            current_left_count++;
        }

        // Compute right error.
        current_right_count = 0;
        for(int i = left_side + 1; i < no_dup_count; i++) {
            al_get(&datapoints_no_dup, i, no_dup_buffer);
            double deviation = no_dup_buffer[1] - right_y_avg;
            double sq_deviation = deviation * deviation;

            double running_err_frac = current_right_count / (current_right_count + 1.0);
            double current_sqdev_frac = 1 / (current_right_count + 1.0);

            error_right = running_err_frac * error_right + current_sqdev_frac * sq_deviation;
            // error_right += deviation * deviation;
            current_right_count++;
        }

        const int total_left_count = current_left_count;
        const int total_right_count = current_right_count;

        // double total_error = (((double)total_left_count) / no_dup_count) * error_left + 
        //                         (((double)total_right_count) / no_dup_count) * error_right;

        double total_error = (total_left_count * error_left + total_right_count * error_right) / no_dup_count;

        if(total_error < lowest_error) {
            lowest_error = total_error;
            lowest_error_lindex = left_side;
        }

        // printf("Current error: %lf\n", total_error);
    }

    // printf("Index of element after which we split: %d\n", lowest_error_lindex);

    // Now we want to calculate the midpoint
    if(lowest_error_lindex != -1) {
        al_get(&datapoints_no_dup, lowest_error_lindex, no_dup_buffer);
        double xval_lower = no_dup_buffer[0];
        al_get(&datapoints_no_dup, lowest_error_lindex + 1, no_dup_buffer);
        double xval_higher = no_dup_buffer[0];

        optimal_threshold = (xval_higher + xval_lower) / 2;
    } else {
        optimal_threshold = DBL_MIN;        // Whoops, there was only 1 datapoint! This will let the system know 
                                            // that a single vector was passed in, which is a bad sign.
    }

    al_destroy(&datapoints_no_dup);
    free(no_dup_buffer);
    free(old_buffer);

    return optimal_threshold;
}

void dtree_setmode_depth(dtree_t * tree, unsigned int depth) {
    tree->trucate_mode = TRUNC_BY_DEPTH;
    tree->depth = depth;
}

void dtree_setmode_samplesize(dtree_t * tree, unsigned int samplesize) {
    tree->trucate_mode = TRUNC_BY_SAMPLE_SIZE;
    tree->sample_size = samplesize;
}

void dtreenode_delete(dtreenode_t * head) {
    // TODO: Finish
    if(head) {
        dtreenode_t * left_ptr = head->left;
        dtreenode_t * right_ptr = head->right;
        
        dtreenode_delete(left_ptr);
        free(head);
        dtreenode_delete(right_ptr);
    }
}

void dtree_destroy(dtree_t * tree) {
    dtreenode_delete(tree->head_ptr);

    tree->depth = 0;
    tree->head_ptr = NULL;
    tree->trucate_mode = UNKNOWN;
    tree->num_vars = 0;
}

void dtree_print(dtree_t tree) {
    arraylist_t queue;
    al_init(&queue, sizeof(dtreenode_t *));

    dtreenode_t * ptr = tree.head_ptr;          // Use this as our buffer
    al_insert(&queue, queue.al_currentsize, &ptr);

    while(queue.al_currentsize > 0) {
        al_get(&queue, 0, &ptr);
        al_remove(&queue, 0);

        if(!ptr) continue;

        int num_tabs = ptr->depth;

        for(int i = 0; i < num_tabs; i++) {
            printf("   ");
        }
        printf("Variable to split on: %d\nThreshold: %lf\nMean for this data: %lf\n\n", 
                ptr->var, ptr->threshold, ptr->average);

        al_insert(&queue, queue.al_currentsize, &(ptr->left));
        al_insert(&queue, queue.al_currentsize, &(ptr->right));
    }

    al_destroy(&queue);
}

double dtree_apply(dtree_t * tree, double * input_vector, unsigned int num_elem) {
    dtreenode_t * ptr = tree->head_ptr;

    while(ptr->var != -1) {
        int var_to_split = ptr->var;
        double xval = input_vector[var_to_split];

        if(xval < ptr->threshold) {
            if(!ptr->left) break;
            ptr = ptr->left;
        } else {
            if(!ptr->right) break;
            ptr = ptr->right;
        }
    }

    return ptr->average;
}

double dtree_get_error(dtree_t * tree, arraylist_t * x_test, arraylist_t * y_test) {
    double total_err = 0;

    double * buf = malloc(x_test->al_elemsize);
    for(int i = 0; i < x_test->al_currentsize; i++) {
        al_get(x_test, i, buf);
        double actual_y;

        al_get(y_test, i, &actual_y);
        double model_y = dtree_apply(tree, buf, x_test->al_elemsize / sizeof(double));

        double dev = actual_y - model_y;
        total_err += dev * dev;
    }
    free(buf);

    return total_err / x_test->al_currentsize;
}