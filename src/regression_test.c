#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"
#include "stringappend.h"
#include "stat.h"
#include "linear_regression.h"

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

    arraylist_t xtest, ytest;
    al_get_from_file(&xtest, xtestfile);
    al_get_from_file(&ytest, ytestfile);

    regression_model_t model;
    regmodel_init(&model, Xs.al_elemsize / sizeof(double));
    regmodel_fit_inv(&model, &Xs, &Ys);

    double regression_error = regmodel_get_error(&model, &xtest, &ytest);

    double sample_mean = dpoint_avg_of_index(&Ys, 0);
    double const_error = constant_get_error(sample_mean, &xtest, &ytest);

    for(int i = 0; i < model.num_vars + 1; i++) {
        printf("%lf\n", model.weights[i]);
    }

    printf("Model loss: %lf; Loss on constant sample mean: %lf\n", regression_error, const_error);

    regmodel_destroy(&model);

    al_destroy(&Xs);
    al_destroy(&Ys);

    al_destroy(&xtest);
    al_destroy(&ytest);

    free(xtestfile);
    free(ytestfile);
    free(xfile);
    free(yfile);

    return EXIT_SUCCESS;
}