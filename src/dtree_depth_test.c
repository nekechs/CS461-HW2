#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arraylist.h"
#include "stringappend.h"
#include "decisiontree.h"

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

    dtree_t tree;
    // dtree_setmode_depth(&tree, 9);
    // dtree_setmode_samplesize(&tree, 10);
    // dtree_fit(&tree, Xs.al_elemsize / sizeof(double), &Xs, &Ys, NULL, NULL);

    // dtree_print(tree);

    arraylist_t xtest, ytest;
    al_get_from_file(&xtest, xtestfile);
    al_get_from_file(&ytest, ytestfile);

    // double d = dtree_get_error(&tree, &xtest, &ytest);
    // printf("%lf\n", d);

    FILE * train_vals = fopen("../output/dtree_depth_error_test", "w");
    FILE * test_vals = fopen("../output/dtree_depth_error_train", "w");
    for(int i = 0; i < 30; i++) {
        time_t t1 = time(NULL);
        dtree_setmode_depth(&tree, i);
        // dtree_setmode_samplesize(&tree, i);
        dtree_fit(&tree, Xs.al_elemsize / sizeof(double), &Xs, &Ys, NULL, NULL);
        double test_err = dtree_get_error(&tree, &xtest, &ytest);
        double train_err = dtree_get_error(&tree, &Xs, &Ys);

        fprintf(train_vals,"%d\t%lf\n",i,test_err);
        fprintf(test_vals,"%d\t%lf\n",i,train_err);
        // printf(time(NULL));
        time_t t2 = time(NULL);
        printf("%d\n", t2 - t1);
    }

    fclose(train_vals);
    fclose(test_vals);

    al_destroy(&Xs);
    al_destroy(&Ys);
    al_destroy(&xtest);
    al_destroy(&ytest);

    free(xfile);
    free(yfile);
    free(xtestfile);
    free(ytestfile);

    dtree_destroy(&tree);
    return EXIT_SUCCESS;
}