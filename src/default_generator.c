#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "random.h"
#include "fileio.h"
#include "arraylist.h"
#include "stringappend.h"
#include "dataset_generator.h"

#define NUM_ENTRIES 100

void create_buffer(double * buffer, unsigned int total_num);
double output_from_buffer(double * buffer, unsigned int total_num);

// First argument: number of data entries
// Second argument: prefix
int main(int argc, char ** argv) {
    size_t num_entries = NUM_ENTRIES;
    char * prefix = append_to_string("default", "");

    srand(time(NULL));

    if(argc == 1 + 2) {
        char * argument = argv[1];
        int number = atoi(argument);
        if(!number) {
            write_stderr("Bruh this is not a number of data entries\n");
        } else if(number <= 0) {
            write_stderr("Bruh: Wrong number of data points\n");
        } else {
            num_entries = number;
        }

        free(prefix);
        prefix = append_to_string(argv[2], "");
    } // else {
    //     x_dim = 10;
    //     num_entries = 10000;
    //     prefix = append_to_string("default", "");   // MAKE SURE THIS IS FREED LATER
    // }

    puts(prefix);

    arraylist_t A;
    arraylist_t B;

    gen_hw2_xvals(&A, num_entries);
    gen_hw2_yvals(&A, 0.05, &B);

    char * input_data_file = append_to_dir(OUTPUT_DIR, prefix);
    append_after(&input_data_file, "_xvals");
    char * output_data_file = append_to_dir(OUTPUT_DIR, prefix);
    append_after(&output_data_file, "_yvals");

    al_save_to_file(&A, input_data_file);
    al_save_to_file(&B, output_data_file);

    // print_double_vect_al(&A);
    // printf("\n");
    // print_double_vect_al(&B);

    al_destroy(&A);
    al_destroy(&B);

    free(input_data_file);  // Frees the string that stores the path/to/file.txt
    free(output_data_file);
    free(prefix);
    return EXIT_SUCCESS;
}