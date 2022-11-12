#ifndef _CLASS_TREE_H_
#define _CLASS_TREE_H_

#include <stdbool.h>

#include "arraylist.h"

#ifndef DEFAULT_MAX_DEPTH
#define DEFAULT_MAX_DEPTH 5
#endif

/* Sets a node's parent, left ref, right ref, and depth */
#define CTREENODE_INIT(node, p, l, r, d) do {   \
    (node).parent = p;   \
    (node).left = l;       \
    (node).right = r;     \
    (node).depth = d;     \
} while (0)

enum trunc_mode {
    UNKNOWN = 0,
    TRUNC_BY_DEPTH,
    TRUNC_BY_SAMPLE_SIZE
};

enum interpret_mode {
    AS_CLASS = 0,
    AS_PROBABILITY,
};

// Struct that represents a node on a decision tree. May or may not be a split here.
struct ctreenode_struct {
    /* Represents a variable to split on */
    int partition_var;

    /* Depth of a given node */
    unsigned int depth;

    /* Relational information regarding the tree structure */
    struct ctreenode_struct * parent;
    struct ctreenode_struct * left;     // Represents the node to go to if the variable is -1
    struct ctreenode_struct * right;    // Represents the node to go to if the variable is +1

    /* Stores the result for this node. */
    double result;
};

typedef struct ctreenode_struct ctreenode_t;

// Struct that represents the actual decision tree.
// Keeps track of valuable information such as 
typedef struct {
    /* Various parameters set for the tree */
    unsigned int num_vars;
    unsigned int max_depth;
    unsigned int min_sample_size;

    /* Stores the head node of the tree */
    ctreenode_t * head_ptr;

    /* Sets the mode for when to stop splitting nodes */
    enum trunc_mode truncate_mode;

    /* Sets whether or not to interpret the result as a probability or as the most likely class */
    enum interpret_mode result_mode;
} ctree_t;

/* Both of these functions set the mode of the trunctation along with the necessary parameters */
void ctree_setmode_depth(ctree_t * tree, unsigned int depth);
void ctree_setmode_samplesize(ctree_t * tree, unsigned int sample_size);

// Given some training data about various binary variables and their binary classes,
// this function fits a classification tree to said training data. Make sure your
// tree structure itself is allocated in memory, but also make sure that the tree
// structure is NOT initialized by some other call to ctree_fit().
void ctree_fit(ctree_t * tree, int num_vars,
                arraylist_t * train_x, arraylist_t * train_y);

// Destroys (but doesn't deallocate) the tree structure. Gets rid of all nodes
// so they don't take any space on the heap.
void ctree_destroy(ctree_t * tree);

void ctree_print(ctree_t tree);

// Uses the classification tree to get a possible classification estimate based on
// the given input data.
double ctree_apply(ctree_t * tree, double * input_vector, unsigned int num_elem);

int ctree_get_misclassified(ctree_t * tree, arraylist_t * x_test, arraylist_t * y_test);

// This is a bit of a hardcoded function: Counts the number of var splits that are
// not in the range (1 to 5, but this will check from 0 to 4 because of 0 indexing)
int ctree_get_irrelevant_num(ctree_t * tree);

#endif