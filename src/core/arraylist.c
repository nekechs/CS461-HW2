#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "arraylist.h"
#include "stringappend.h"

// TODO: MAJOR FUCKING TODO HERE: Make sure this works using some driver file.

void al_init(arraylist_t * A, size_t al_elemsize) {
    A->data = malloc(DEFAULT_SIZE * al_elemsize);
    A->al_capacity = DEFAULT_SIZE;
    A->al_elemsize = al_elemsize;
    
    A->al_currentsize = 0;
}

void al_destroy(arraylist_t * A) {
    free(A->data);
    A->al_capacity = 0;
    A->al_elemsize = 0;
    A->al_currentsize = 0;
}

void al_insert(arraylist_t * A, unsigned long int index, char * elem_data) {
    if(index > A->al_currentsize) index = A->al_currentsize;

    if(A->al_capacity == A->al_currentsize) {
        // Now we know it is time to resize.
        char * tmp_ref = A->data;

        A->data = (char * ) malloc(2 * A->al_capacity * A->al_elemsize);
        memcpy(A->data, tmp_ref, A->al_capacity * A->al_elemsize);

        free(tmp_ref);

        A->al_capacity *= 2;
    }
    char * data_ref = A->data;
    const size_t elemsize = A->al_elemsize;

    // Move the data by 1 to the right.
    // 0 1  2  3  4  5  6  7  8
    // 5 10 15 20 25 30 35 40
    // 5 10 15 20 xx 25 30 35 40
    memmove(data_ref + (index + 1) * elemsize, data_ref + index * elemsize, elemsize * (A->al_currentsize - index));
    // for(int i = A->al_currentsize; i > index; i--) {
    //     *(data_ref + i * elemsize) = *(data_ref + (i - 1) * elemsize);
    // }

    memcpy(data_ref + index * elemsize, elem_data, elemsize);

    A->al_currentsize++;
}

void al_remove(arraylist_t * A, unsigned long int index) {
    const size_t oldsize = A->al_currentsize;

    if(index >= oldsize) {
        index = oldsize - 1;
    }

    const size_t elemsize = A->al_elemsize;
    char * data_ref = A->data;

    memmove(data_ref + index * elemsize, data_ref + (index + 1) * elemsize, elemsize * (A->al_currentsize - 1 - index));
    // for(int i = index; i < oldsize - 1; i++) {
    //     *(data_ref + i * elemsize) = *(data_ref + (i + 1) * elemsize);
    // }

    A->al_currentsize--;
}

void al_get(arraylist_t * A, unsigned long int index, void * retrieved_data){ 
    memcpy(retrieved_data, A->data + index * A->al_elemsize, A->al_elemsize);
}

void al_trim(arraylist_t * A){
    const size_t new_capacity = A->al_currentsize;
    const size_t elemsize = A->al_elemsize;

    char * tmp_ref = A->data;

    A->data = (char * ) malloc(new_capacity * elemsize);
    memcpy(A->data, tmp_ref, new_capacity * elemsize);

    free(tmp_ref);

    A->al_capacity = new_capacity;
}

int al_save_to_file(arraylist_t * A, char * filename) {
    FILE * fp = fopen(filename, "w");

    size_t es_cs_cp[3] = {A->al_elemsize, A->al_currentsize, A->al_capacity};
    fwrite(es_cs_cp, sizeof(size_t), 3, fp);
    fwrite(A->data, A->al_elemsize, A->al_currentsize, fp);

    fclose(fp);

    return 0;
}

int al_get_from_file(arraylist_t * A, char * filename) {
    FILE * fp = fopen(filename, "r");

    // puts("s");

    if(fp == NULL) {
        perror("Error opening a file.");
        return -1;
    }

    size_t es_cs_cp[3];

    fread(es_cs_cp, sizeof(size_t), 3, fp);

    // printf("%d, %d, %d\n", es_cs_cp[0], es_cs_cp[1], es_cs_cp[2]);
    A->al_elemsize = es_cs_cp[0];
    A->al_currentsize = es_cs_cp[1];
    A->al_capacity = es_cs_cp[2];

    A->data = malloc(A->al_elemsize * A->al_capacity);
    fread(A->data, A->al_elemsize, A->al_currentsize, fp);

    return -0;
}

int al_get_num_doubles(arraylist_t * dvect_list) {
    return (dvect_list->al_elemsize + sizeof(double) - 1) / (sizeof(double));
}

void print_double_elem(void * data, int num_doubles) {
    double * double_array = (double *) data;
    for(int i = 0; i < num_doubles; i++) {
        printf("%.3lf\t", double_array[i]);
    }
    printf("\n");
}

void print_double_vect_al(arraylist_t * dvect_list) {
    unsigned int num_doubles = (dvect_list->al_elemsize + sizeof(double) - 1) / (sizeof(double));

    double * buffer = calloc(num_doubles, sizeof(double));
    for(int i = 0; i < dvect_list->al_currentsize; i++) {
        al_get(dvect_list, i, buffer);
        print_double_elem(buffer, num_doubles);
    }
    free(buffer);
}