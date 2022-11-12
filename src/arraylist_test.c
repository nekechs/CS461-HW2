#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"
#include "stringappend.h"
#include "decisiontree.h"
#include "stat.h"

#ifndef OUTPUT_DIR
#define OUTPUT_DIR "../output/"
#endif

int main(int argc, char ** argv) {
    arraylist_t Xs, Ys;

    char * xfile = append_to_dir(OUTPUT_DIR, "training_xvals");
    char * yfile = append_to_dir(OUTPUT_DIR, "training_yvals");
    char * xtestfile = append_to_dir(OUTPUT_DIR, "testing_xvals");
    char * ytestfile = append_to_dir(OUTPUT_DIR, "testing_yvals");


    if(al_get_from_file(&Xs, xfile)) return EXIT_FAILURE;
    if(al_get_from_file(&Ys, yfile)) return EXIT_FAILURE;
    // puts("s");

    double sample_mean = dpoint_avg_of_index(&Ys, 0);
    double training_err = constant_get_error(sample_mean, &Xs, &Ys);

    al_destroy(&Xs);
    al_destroy(&Ys);

    arraylist_t xtest, ytest;
    al_get_from_file(&xtest, xtestfile);
    al_get_from_file(&ytest, ytestfile);

    sample_mean = dpoint_avg_of_index(&ytest, 0);
    double testing_err = constant_get_error(sample_mean, &xtest, &ytest);
    printf("Testing error: %lf; Training error: %lf\n", testing_err, training_err);

    al_destroy(&xtest);
    al_destroy(&ytest);
    free(xfile);
    free(yfile);
    free(xtestfile);
    free(ytestfile);

    return EXIT_SUCCESS;
}
