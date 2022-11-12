#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

#include <stdlib.h>

#define DEFAULT_SIZE 16

typedef struct {
    char * data;                // all of the data
    size_t al_elemsize;           // element size in bytes

    size_t al_currentsize;        // Total number of elements stored in this arraylist
    size_t al_capacity;
} arraylist_t;

typedef struct {
    void * ptr;
    size_t slice_size;
} al_slice_t;

// Initializes an arraylist object based on some struct pointer. This will allocate
// space in the heap.
void al_init(arraylist_t * A, size_t al_elemsize);

// Frees the arraylist's data from memory. If the the arraylist is destroyed,
// we will not have any more memory leaks coming from all the allocated data.
void al_destroy(arraylist_t * A);

// Inserts an element into the arraylist. Will automatically increase capacity
// if there is not enough space left.
void al_insert(arraylist_t * A, unsigned long int index, char * elem_data);

// Removes an element from the arraylist at the given index. Does not reduce the size
// of the arraylist.
void al_remove(arraylist_t * A, unsigned long int index);

// Retreive the data from the list at a given index. Puts the data into a void pointer
// that should have enough space to retrieve the data. Make sure that this is the case!
void al_get(arraylist_t * A, unsigned long int index, void * retrieved_data);

// Instead of copying the data to some destination pointer, this one just gets the raw
// pointer to where the element's data is actually stored.
al_slice_t al_get_slice(arraylist_t * A, unsigned long int index);

// Trims an arraylist down to the capacity. Note that this does indeed reallocate
// the memory.
void al_trim(arraylist_t * A);

// Saves the data of the arraylist to a file, as well as the metadata.
// If there is an error, this function will return the error.
// If return 0: no error saving to file.
int al_save_to_file(arraylist_t * A, char * filename);

// Initializes an arraylist object from memory. Must eventually destroy
// the arraylist object at some point.
// If return 0: No error reading from file.
// DO NOT USE THIS ON AN INITIALIZED ARRAYLIST
int al_get_from_file(arraylist_t * A, char * filename);

// If you have a list of double vectors, this function will compute the number
// of doubles in a vector using the size of each element. This function does actually
// account for a size that is not a multiple of sizeof(double) by rounding down.
int al_get_num_doubles(arraylist_t * dvect_list);

void print_double_elem(void * data, int num_doubles);

void print_double_vect_al(arraylist_t * dvect_list);

#endif