#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdbool.h>

#include "fileio.h"
#include "classtree.h"
#include "arraylist.h"
#include "linkedlist.h"
#include "stat.h"
#include "assert.h"

#ifndef DEBUG
#define DEBUG 0
#endif

void ctreenode_recursive_delete(ctreenode_t * head);

/**
 * @brief Recursive function that takes some node in the decision tree, training data
 * relevant to that node, and then gives the node left and right children that
 * correspond to splitting on the optimal variable. 
 * 
 * @param head the node you want to add children to
 * @param relevant_datapoints the datapoints that will tell you what var to split on
 * @param orig_tree The original tree contains relevant information. Please include.
 */
void split_under_node(ctreenode_t * head, arraylist_t * relevant_datapoints,
                        ctree_t * orig_tree);

void ctree_fit(ctree_t * tree, int num_vars,
                arraylist_t * train_x, arraylist_t * train_y) {
    // TODO: Finish
    if(train_x->al_currentsize != train_y -> al_currentsize) {
        // There has been an error.
        write_stderr("For some reason, there has been a mismatch in the num of x values and y values. RIP");
        return;
    }

    if(tree->truncate_mode == UNKNOWN) {
        ctree_setmode_samplesize(tree, DEFAULT_MAX_DEPTH);
    }

    tree->num_vars = num_vars;

    /* This arraylist will store a special type of vector: */
    /* All elements will just be the corresponding vector from train_x with extra element of the end from train_y*/
    arraylist_t datapoints;
    al_init(&datapoints, sizeof(double) * (num_vars + 1));

    double * datapoint_buffer = malloc(sizeof(double) * (num_vars + 1));
    for(int i = 0; i < train_x->al_currentsize; i++) {
        /* This is where format the vector for datapoints arraylist and insert it */
        al_get(train_x, i, datapoint_buffer);
        al_get(train_y, i, datapoint_buffer + num_vars);
        al_insert(&datapoints, datapoints.al_currentsize, datapoint_buffer);
    }
    free(datapoint_buffer);

    ctreenode_t * head = malloc(sizeof(ctreenode_t));
    CTREENODE_INIT(*head, NULL, NULL, NULL, 0);

    /* Here is where you should insert a splitting function */
    split_under_node(head, &datapoints, tree);  // TODO: Make sure this function works!!

    tree->head_ptr = head;

    al_destroy(&datapoints);
}

void ctreenode_recursive_delete(ctreenode_t * head) {
    if(head) {
        ctreenode_t * left_ptr = head->left;
        ctreenode_t * right_ptr = head->right;

        ctreenode_recursive_delete(left_ptr);
        free(head);
        ctreenode_recursive_delete(right_ptr);
    }
}

void ctree_destroy(ctree_t * tree) {
    // TODO: Verify
    ctreenode_recursive_delete(tree->head_ptr);

    tree->max_depth = 0;
    tree->head_ptr = NULL;
    tree->truncate_mode = UNKNOWN;
    tree->num_vars = 0;
}

void ctree_print(ctree_t tree) {
    // TODO: Verify
    arraylist_t queue;
    al_init(&queue, sizeof(ctreenode_t *));

    ctreenode_t * ptr = tree.head_ptr;
    al_insert(&queue, queue.al_currentsize, &ptr);

    while(queue.al_currentsize > 0) {
        al_get(&queue, 0, &ptr);
        al_remove(&queue, 0);

        if(!ptr) continue;

        int num_tabs = ptr->depth;

        for(int i = 0; i < num_tabs; i++) {
            printf("\t");
        }

        printf("Variable to split on: %d\nResult: %lf\n\n",
                ptr->partition_var, ptr->result);
        
        al_insert(&queue, queue.al_currentsize, &(ptr->left));
        al_insert(&queue, queue.al_currentsize, &(ptr->right));
    }

    al_destroy(&queue);   
}

void split_under_node(ctreenode_t * head, arraylist_t * relevant_datapoints,
                        ctree_t * orig_tree) {
    int num_y = 0;
    double * buffer = malloc(relevant_datapoints->al_elemsize);
    for(int i = 0; i < relevant_datapoints->al_currentsize; i++) {
        al_get(relevant_datapoints, i, buffer);
        if(buffer[orig_tree->num_vars] > 0) {
            num_y++;     // We do this because we want to keep track of how many y classes exceed 0
        }
    }
    free(buffer);

    /* Configure the basic results relevant to the node passed into this function*/
    head->result = ((double)num_y) / relevant_datapoints->al_currentsize;
    head->partition_var = -1;

    // printf("Is sample size: %d\n", orig_tree->truncate_mode == TRUNC_BY_SAMPLE_SIZE);
    /* Take into account the stopping conditions. */
    switch(orig_tree->truncate_mode) {
        case TRUNC_BY_DEPTH:
            if(head->depth > orig_tree->max_depth) {
                return;
            }
            break;
        case TRUNC_BY_SAMPLE_SIZE:
            if(relevant_datapoints->al_currentsize < orig_tree->min_sample_size)  {
                // printf("Wow %d is less than %d\n", relevant_datapoints->al_currentsize, orig_tree->min_sample_size);
                return;
            }
            break;
        default:
            orig_tree->truncate_mode = TRUNC_BY_DEPTH;
            orig_tree->max_depth = DEFAULT_MAX_DEPTH;
            if(head->depth > orig_tree->max_depth) {
                return;
            }
    }


    if(num_y != relevant_datapoints->al_currentsize && num_y != 0) {
        /* we pass in var_to_split's reference to the max_ig function to store the var with max ig */
        int var_to_split;
        double max_ig = dpoint_get_max_ig(relevant_datapoints, orig_tree->num_vars, &var_to_split);
        // if(max_ig == 0) return;

        head->partition_var = var_to_split;
        // printf("\nmaximum information gain is %lf at variable %d\n", max_ig, var_to_split);

        arraylist_t left_data;
        arraylist_t right_data;

        al_init(&left_data, relevant_datapoints->al_elemsize);
        al_init(&right_data, relevant_datapoints->al_elemsize);

        double * buffer = malloc(relevant_datapoints->al_elemsize);
        for(int i = 0; i < relevant_datapoints->al_currentsize; i++) {
            al_get(relevant_datapoints, i, buffer);
            if(buffer[var_to_split] < 0) {
                al_insert(&left_data, left_data.al_currentsize, buffer);
            } else {
                al_insert(&right_data, right_data.al_currentsize, buffer);
            }
        }
        free(buffer);

        ctreenode_t * left_head = malloc(sizeof(ctreenode_t));
        CTREENODE_INIT(*left_head, head, NULL, NULL, head->depth + 1);
        split_under_node(left_head, &left_data, orig_tree);
        head->left = left_head;

        ctreenode_t * right_head = malloc(sizeof(ctreenode_t));
        CTREENODE_INIT(*right_head, head, NULL, NULL, head->depth + 1);
        split_under_node(right_head, &right_data, orig_tree);
        head->right = right_head;

        al_destroy(&left_data);
        al_destroy(&right_data);
    }
    
    return;
}

void ctree_setmode_depth(ctree_t * tree, unsigned int depth) {
    tree->truncate_mode = TRUNC_BY_DEPTH;
    tree->max_depth = depth;
}

void ctree_setmode_samplesize(ctree_t * tree, unsigned int samplesize) {
    tree->truncate_mode = TRUNC_BY_SAMPLE_SIZE;
    tree->min_sample_size = samplesize;
}

double ctree_apply(ctree_t * tree, double * input_vector, unsigned int num_elem) {
    ctreenode_t * ptr = tree->head_ptr;
    bool leaf_err_encountered = false;

    while(ptr->partition_var != -1) {
        int var_to_split = ptr->partition_var;
        double xval = input_vector[var_to_split];

        if(!ptr->left != !ptr->right) {
            leaf_err_encountered = true;
            break;
        }

        if(xval < 0) ptr = ptr->left;
        else ptr = ptr->right;

        // if(xval < 0 && !ptr->left) {
        //     if(!ptr->left) {
        //         leaf_err_encountered = true;
        //         break;
        //     }
        //     ptr = ptr->left;
        // } else {
        //     if(!ptr->right) {
        //         leaf_err_encountered = true;
        //         break;
        //     }
        //     ptr = ptr->right;
        // }
    }

    if(leaf_err_encountered) {
        write_stderr("Error: Encountered a non leaf with only one child");
        char buf[512];
        int num = snprintf(buf, 512, "Problem occured at depth %d; Variable # splitted is %d, and var is actually %d",
                            ptr->depth, ptr->partition_var, input_vector[ptr->partition_var]);
        write_stderr(buf);
    }

    double return_value = ptr->result;
    if(tree->result_mode == AS_CLASS) {
        if(ptr->result < 0.5) {
            return_value = -1;
        } else {
            return_value = 1;
        }
    }
    return return_value;
}

int ctree_get_misclassified(ctree_t * tree, arraylist_t * x_test, arraylist_t * y_test) {
    tree->result_mode = AS_CLASS;

    int num_misclassified = 0;

    double * buf = malloc(x_test->al_elemsize);
    for(int i = 0; i < x_test->al_elemsize; i++) {
        al_get(x_test, i, buf);

        double actual_y;
        
        al_get(y_test, i, &actual_y);
        double model_y = ctree_apply(tree, buf, tree->num_vars);

        int diff = (int)(model_y - actual_y);
        if(diff) num_misclassified++;
    }
    free(buf);

    return num_misclassified;
}

int ctreenode_get_irrel_recursive(ctreenode_t * head) {
    if(head->partition_var == -1) {
        return 0;
    }

    int ret_val = ctreenode_get_irrel_recursive(head->left) + 
                    ctreenode_get_irrel_recursive(head->right) + 
                    (head->partition_var >= 5 ? 1 : 0);

    return ret_val;
}

int ctree_get_irrelevant_num(ctree_t * tree) {
    return ctreenode_get_irrel_recursive(tree->head_ptr);
}