#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

void ll_init(linkedlist_t * A, size_t elem_size) {
    A->head = NULL;
    A->ll_length = 0;
    A->elem_size = elem_size;
}

// REQUIREMENT: All nodes in A must be initialized! They must all have data
// segments that are all pointing to allocated memory on the heap.
void ll_destroy(linkedlist_t * A) {
    node_t * headnode = A->head;
    while(headnode != NULL) {
        headnode = node_destroy(headnode);
    }
}

void ll_update_length(linkedlist_t * A) {
    A->ll_length = 0;

    for(node_t * node_ptr = A->head; node_ptr != NULL; node_ptr = node_ptr->next_node) {
        A->ll_length++;
    }
}

void ll_front_insert(linkedlist_t * A, void * data, size_t data_size) {
    node_t * new_node = node_init(data, data_size, A->elem_size);
    new_node->next_node = A->head;
    A->head = new_node;

    A->ll_length++;
}

void ll_append(linkedlist_t * A, void * data, size_t data_size) {
    node_t * new_node = node_init(data, data_size, A->elem_size);
    
    node_t ** final_ptr_reference = &(A->head);
    while(*final_ptr_reference != NULL) {
        final_ptr_reference = &((*final_ptr_reference)->next_node);
    }
   
    *final_ptr_reference = new_node;

    A->ll_length++;
}

node_t * ll_insert_after_node(linkedlist_t * A, node_t * node, void * data, size_t data_size) {
    if(node == NULL && A->head != NULL) {
        perror("Tried to insert after a node that does not exist.");
        exit(EXIT_FAILURE);
    } else if (node == NULL && A->head == NULL) {
        ll_front_insert(A, data, data_size);
        return A->head;
    }
    
    node_t * new_node = node_init(data, data_size, A->elem_size);
    
    new_node->next_node = node->next_node;
    node->next_node = new_node;

    A->ll_length++;

    return new_node;
}

void ll_delete_front(linkedlist_t * A) {
    if(A->head == NULL) return;

    A->head = node_destroy(A->head);

    A->ll_length--;
}

void ll_delete_end(linkedlist_t * A) {
    if(A->head == NULL) return;

    node_t ** final_ptr_reference = &(A->head);

    while( (*final_ptr_reference)->next_node != NULL) {
        final_ptr_reference = &((*final_ptr_reference)->next_node);
    }

    *final_ptr_reference = node_destroy(*final_ptr_reference);

    A->ll_length--;
}

void * node_access_data(node_t * node) {
    return node->node_data;
}

node_t * node_destroy(node_t * node) {
    // Store where the next node is located
    node_t * next_ptr = node->next_node;

    // Destroy the node's data in memory, and then the node itself
    free(node->node_data);
    free(node);

    return next_ptr;
}

node_t * node_init(void * data, size_t data_size, size_t node_capacity) {
    // Allocate space for a new node in memory
    node_t * node = malloc(sizeof(node_t));

    if(data_size > node_capacity) {
        // Error here - specified data_size was more than the capacity!
        perror("Tried to make a node with data of higher size than the capacity.");
        exit(EXIT_FAILURE);
    }
    node->node_capacity = node_capacity;
    node->node_occupied = data_size;

    // Allocate space for data in memory, and then copy over the specified data
    // to the node's storage of data.
    node->node_data = malloc(node_capacity);
    memcpy(node->node_data, data, data_size);

    node->next_node = NULL;

    return node;
}