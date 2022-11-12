#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdlib.h>

/*
typedef struct {
    char * node_data;
    size_t node_capacity;
    size_t node_occupied;
    node_t * next_node;
} node_t;
*/

struct node_struct {
    char * node_data;
    size_t node_capacity;
    size_t node_occupied;
    struct node_struct * next_node;
};

typedef struct node_struct node_t;

/**
 * @brief struct that stores info about a linked list in memory
 * 
 * head - pointer to the head node of the linked list
 * ll_length - length of the linked list in memory
 * elem_size - capacity of how much data each node can hold
 */
typedef struct {
    node_t * head;
    size_t ll_length;
    size_t elem_size;
} linkedlist_t;
/**
 * @brief Initializes a pointer to a linked list object in memory
 * 
 * @param a - pointer to linked list object
 * @param elem_size - size of the data stored in each element (in bytes)
 */
void ll_init(linkedlist_t * a, size_t elem_size);

/**
 * @brief Completely destroys a linked list object in memory. The implications
 * of this are that it frees all of the elements in the linked list and does not
 * create any memory leaks.
 * 
 * @param a - linked list to be destroyed
 */
void ll_destroy(linkedlist_t * a);

/**
 * @brief Returns the number of elements in a linked list object in memory
 * 
 * @param a - Linked list that you want the length of
 * @return size_t - length of the linked list
 */
size_t ll_length(linkedlist_t * a);

/**
 * @brief Updates the length of a linked list - basically, it goes through the entire
 * linked list and updates the size of the ll_length parameter of the list.
 * 
 * @param a - linked list that you wanna update
 */
void ll_update_length(linkedlist_t * a);

/**
 * @brief Inserts a piece of data as a new node at the FRONT of the linked list.
 * Note: the data MUST have the same size as all of the elements in the linked list.
 * 
 * @param a - pointer to the linked list object
 * @param data - pointer to the bytes of data
 * @param data_size - size of the data in bytes
 */
void ll_front_insert(linkedlist_t * a, void * data, size_t data_size);

/**
 * @brief Appends a piece of data - now if you append the data, it becomes the LAST
 * node in the list. 
 * Note: the data MUST have the same size as all of the elements in the linked list.
 * 
 * @param a - pointer to the linked list object
 * @param data - pointer to the bytes of data
 * @param data_size - size of the data in bytes
 */
void ll_append(linkedlist_t * a, void * data, size_t data_size);

/**
 * @brief Inserts a piece of data after a given node reference in the linked list.
 * Note: the data MUST be at MOST the size of the capcity of all nodes in the linked
 * list.
 * 
 * REQUIREMENT - node_t * node MUST point to an actual node on the heap. It CANNOT
 * be NULL, unless the linked list is empty.
 * 
 * @param a - pointer to the linked list object
 * @param node - pointer to the node after which you want to insert a new node
 * @param data - the data which you want to actually put in the new node
 * @param data_size - the size of the data (in bytes)
 * @return node_t* - pointer to the newly made node
 */
node_t * ll_insert_after_node(linkedlist_t * a, node_t * node, void * data, size_t data_size);

/**
 * @brief Deletes the front element of the linked list.
 * 
 * @param a - a pointer to the actual linked list object
 */
void ll_delete_front(linkedlist_t * a);

/**
 * @brief Deletes the last element of a linked list.
 * 
 * @param a - a pointer to the actual linked list object
 */
void ll_delete_end(linkedlist_t * a);

/**
 * @brief Gets the next node based on the current inputted node
 * 
 * @param node - the node you want to examine
 * @return node_t* - a pointer to the next node
 */
node_t * node_next(node_t * node);

/**
 * @brief Accesses the memory stored in a certain node (probably somewhere in a linked list)
 * 
 * @param node - the node whose data you want to access
 * @return void* - a reference to the relevant data
 */
void * node_access_data(node_t * node); 

/**
 * @brief Destroys a given node, and returns a pointer to whatever the given node
 * was pointing to. Note: node MUST Be on the heap!!!!! Otherwise you would free a
 * pointer not on the heap.
 * 
 * @param node - pointer to the node in memory
 * @return node_t* - pointer to where this node was pointing to
 */
node_t * node_destroy(node_t * node);

/**
 * @brief Creates a new node in memory and then copies all bytes form the data
 * pointer over to some space in memory (pointed to by node->node_data) 
 * 
 * Note: data_size MUST be equal to or less than node_capacity. Otherwise this
 * function will fail and exit with EXIT_FAILURE.
 * 
 * @param data - pointer to the data which you want to store
 * @param data_size - size of the data you want to copy over
 * @param node_capacity - capacity of the node (MUST BE >= data_size)
 * @return node_t* - pointer to a dynamically allocated node on the heap
 */
node_t * node_init(void * data, size_t data_size, size_t node_capacity);
#endif