#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arraylist.h"
#include "stringappend.h"
#include "classtree.h"

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

    ctree_t tree;
    tree.result_mode = AS_CLASS;
    // dtree_setmode_depth(&tree, 9);
    // dtree_setmode_samplesize(&tree, 10);
    // dtree_fit(&tree, Xs.al_elemsize / sizeof(double), &Xs, &Ys, NULL, NULL);

    // dtree_print(tree);

    arraylist_t xtest, ytest;
    al_get_from_file(&xtest, xtestfile);
    al_get_from_file(&ytest, ytestfile);

    int prev_test_err = 0;
    int prev_i = -1;

    FILE * train_vals = fopen("../output/ctree_ssize_error_train", "w");
    FILE * test_vals = fopen("../output/ctree_ssize_error_test", "w");
    for(int i = 1; i < 2499; i+= 2) {
        time_t t1 = time(NULL);
        ctree_setmode_samplesize(&tree, i);
        ctree_fit(&tree, Xs.al_elemsize / sizeof(double), &Xs, &Ys);
        int test_err = ctree_get_misclassified(&tree, &xtest, &ytest);
        int train_err = ctree_get_misclassified(&tree, &Xs, &Ys);

        fprintf(train_vals,"%d\t%d\n",i,train_err);
        fprintf(test_vals,"%d\t%d\n",i,test_err);
        time_t t2 = time(NULL);
        // printf("%d\n", t2 - t1);
        if(prev_test_err == 0 && test_err != 0) printf("%d\n", prev_i);

        prev_test_err = test_err;
        prev_i = i;
        
        ctree_destroy(&tree);
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

    // ctree_destroy(&tree);
    return EXIT_SUCCESS;
}