#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "random.h"
#include "fileio.h"
#include "linkedlist.h"
#include "arraylist.h"
#include "stringappend.h"

#define NUM_ENTRIES 100

void create_buffer(double * buffer, unsigned int total_num);
double output_from_buffer(double * buffer, unsigned int total_num);

// First argument: d
// Second argument: number of data entries
// Third argument: prefix
int main(int argc, char ** argv) {
    unsigned int x_dim = 5;
    size_t num_entries = NUM_ENTRIES;
    char * prefix = append_to_string("default", "");

    srand(time(NULL));

    if(argc == 1 + 3) {
        char * argument = argv[1];
        int number = atoi(argument);
        if(!number) {
            write_stderr("Error: The entered argument is not a string.\n");
        } else if (number < 5) {
            write_stderr("Error: The entered argument is less than 5.\n");
        } else {
            x_dim = number;
        }

        argument = argv[2];
        number = atoi(argument);
        if(!number) {
            write_stderr("Bruh this is not a number of data entries\n");
        } else if(number <= 0) {
            write_stderr("Bruh: Wrong number of data points\n");
        } else {
            num_entries = number;
        }

        free(prefix);
        prefix = append_to_string(argv[3], "");
    } // else {
    //     x_dim = 10;
    //     num_entries = 10000;
    //     prefix = append_to_string("default", "");   // MAKE SURE THIS IS FREED LATER
    // }

    puts(prefix);

    arraylist_t A;
    arraylist_t B;

    al_init(&A, sizeof(double) * x_dim);
    al_init(&B, sizeof(double));

    double * _input_buffer = malloc(x_dim * sizeof(double));
    for(int i = 0; i < num_entries; i++) {
        create_buffer(_input_buffer, x_dim);
        al_insert(&A, A.al_currentsize, _input_buffer);

        double temp_double = output_from_buffer(_input_buffer, x_dim);
        al_insert(&B, A.al_currentsize, &temp_double);
    }
    free(_input_buffer);

    char * input_data_file = append_to_dir(OUTPUT_DIR, prefix);
    append_after(&input_data_file, "_xvals");
    char * output_data_file = append_to_dir(OUTPUT_DIR, prefix);
    append_after(&output_data_file, "_yvals");

    al_save_to_file(&A, input_data_file);
    al_save_to_file(&B, output_data_file);

    al_destroy(&A);
    al_destroy(&B);

    free(input_data_file);  // Frees the string that stores the path/to/file.txt
    free(output_data_file);
    free(prefix);
    return EXIT_SUCCESS;
}

double output_from_buffer(double * buf, unsigned int total_num) {
    double x1 = buf[0];
    double x2 = buf[1];
    double x3 = buf[2];
    double x4 = buf[3];
    double x5 = buf[4];

    return 4 - 3 * (x1 * x1 /*x1^2*/) + x3 - 0.01 * x4 + x2 * x5 + normal_generator(0, 0.01);
}

/**
 * @brief Initializes a buffer object to be inserted into an arraylist of data vectors.
 * 
 * @param buffer The actual buffer to be inserted into the arraylist
 * @param excess The total number of numbers in a data vector
 */
void create_buffer(double * buffer, unsigned int total_num) {
    buffer[0] = normal_generator(3.0, 1.0);
    buffer[1] = normal_generator(-2.0, 1.0);
    buffer[2] = buffer[0] + 2 * buffer[1];
    buffer[3] = pow(buffer[1] + 2, 2);
    buffer[4] = bernoulli_generator(0.8);
    for(int i = 5; i < total_num; i++) {
        buffer[i] = normal_generator(0, 1);
    }
}