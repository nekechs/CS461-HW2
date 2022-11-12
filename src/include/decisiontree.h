#ifndef _DECISION_TREE_H_
#define _DECISION_TREE_H_

#ifndef DEFAULT_MAX_DEPTH
#define DEFAULT_MAX_DEPTH 5
#endif


#include "arraylist.h"

enum trunc_mode {
    UNKNOWN = 0,
    TRUNC_BY_DEPTH,
    TRUNC_BY_SAMPLE_SIZE
};

struct dtreenode_struct {
    int var;   // This is the variable to split on
    double threshold;   // This is the threshold that decides left vs right

    unsigned int depth;

    struct dtreenode_struct * parent;
    struct dtreenode_struct * left;
    struct dtreenode_struct * right;

    double average;
};

typedef struct dtreenode_struct dtreenode_t;

typedef struct {
    unsigned int num_vars;
    unsigned int depth;
    unsigned int sample_size;

    dtreenode_t * head_ptr;
    enum trunc_mode trucate_mode;
} dtree_t;

/**
 * @brief This sets the decision tree's mode equal to depth, and sets the actual depth value. 
 * 
 * @param tree 
 * @param depth 
 */
void dtree_setmode_depth(dtree_t * tree, unsigned int depth);

void dtree_setmode_samplesize(dtree_t * tree, unsigned int sample_size);

/**
 * @brief Creates a decision tree based on training data and testing data. Also, 
 * cuts off the learning when a certain condition is met. This function WILL
 * allocate memory for the tree structure.
 * 
 * @param tree The decision tree object that you want to add to
 * @param num_vars The number of variables for each feature of X
 * @param train_x The training data inputs as an arraylist
 * @param train_y The training data outputs as an arraylist
 * @param test_x The test data inputs as an arraylist
 * @param test_y The test data outputs as an arraylist
 */
void dtree_fit(dtree_t * tree, int num_vars,
                arraylist_t * train_x, arraylist_t * train_y,
                arraylist_t * test_x, arraylist_t * test_y);

/**
 * @brief Destroys a decision tree from memory entirely. No memory leaks. 
 * 
 * @param tree Pointer to the decision tree object that is to be deleted 
 */
void dtree_destroy(dtree_t * tree);

/**
 * @brief Prints out the stats for the decision tree. 
 * 
 * @param tree The tree that is to be printed
 */
void dtree_print(dtree_t tree);

/**
 * @brief Uses the decision tree to get a estimate of the output based on a given input. 
 * 
 * @param tree The decision tree to be used (MUST BE INITIALIZED AND FITTED IN MEMORY)
 * @param input_vector Contains an array of doubles to apply as input to the model
 * @return double The result of the model
 */
double dtree_apply(dtree_t * tree, double * input_vector, unsigned int num_elem);

double dtree_get_error(dtree_t * tree, arraylist_t * xtest, arraylist_t * ytest);

#endif