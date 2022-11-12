#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "random.h"
#include "fileio.h"
#include "linkedlist.h"
#include "stringappend.h"

#define NUM_TRIALS 30
#define P 0.55

int main(int argc, char ** argv) {
    srand( time(NULL) );

    // This will store the trials
    int trials[NUM_TRIALS];
    int sum = 0;

    for(int i = 0; i < NUM_TRIALS; i++) {
        trials[i] = bernoulli_generator(P);
        sum += trials[i];
    }

    // This is our average estimate
    double p_hat = (double)sum / NUM_TRIALS;

    printf("Bruh %s\n", OUTPUT_DIR);

    char * output_file = append_to_dir(OUTPUT_DIR, "1-3.txt");
    FILE * file_ptr = fopen(output_file, "w");

    for(int i = 0; i < NUM_TRIALS; i++) {
        fprintf(file_ptr, "%d\n", trials[i]);
    }

    fprintf(file_ptr, "\n");
    fprintf(file_ptr, "Value of p_hat: %lf\n", p_hat);

    fclose(file_ptr);
    free(output_file);

    return EXIT_SUCCESS;
}