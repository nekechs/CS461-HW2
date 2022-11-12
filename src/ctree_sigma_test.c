#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arraylist.h"
#include "stringappend.h"
#include "classtree.h"
#include "dataset_generator.h"

#ifndef OUTPUT_DIR
#define OUTPUT_DIR "../output/"
#endif

int main(int argc, char ** argv) {
    arraylist_t Xs;

    char * xfile = append_to_dir(OUTPUT_DIR, "training_xvals");
    char * yfile = append_to_dir(OUTPUT_DIR, "training_yvals");
    char * xtestfile = append_to_dir(OUTPUT_DIR, "testing_xvals");
    char * ytestfile = append_to_dir(OUTPUT_DIR, "testing_yvals");


    if(al_get_from_file(&Xs, xfile)) return EXIT_FAILURE;
    // if(al_get_from_file(&Ys, yfile)) return EXIT_FAILURE;

    ctree_t tree;
    tree.result_mode = AS_CLASS;

    arraylist_t xtest;
    al_get_from_file(&xtest, xtestfile);
    // al_get_from_file(&ytest, ytestfile);

    int prev_test_err = 0;
    int prev_i = -1;

    FILE * train_vals = fopen("../output/ctree_sigma_error_train", "w");
    FILE * test_vals = fopen("../output/ctree_sigma_error_test", "w");
    FILE * irrel_feature_vals = fopen("../output/ctree_irrel_features", "w");
    for(double sigma = 0; sigma < 1; sigma += 0.05) {
        arraylist_t Ys;
        arraylist_t ytest;

        gen_hw2_yvals(&Xs, sigma, &Ys);
        gen_hw2_yvals(&xtest, sigma, &ytest);

        ctree_setmode_samplesize(&tree, 10);
        ctree_fit(&tree, Xs.al_elemsize / sizeof(double), &Xs, &Ys);
        int test_err = ctree_get_misclassified(&tree, &xtest, &ytest);
        int train_err = ctree_get_misclassified(&tree, &Xs, &Ys);
        int irrel_num = ctree_get_irrelevant_num(&tree);

        fprintf(train_vals,"%lf\t%d\n", sigma, train_err);
        fprintf(test_vals,"%lf\t%d\n", sigma, test_err);
        fprintf(irrel_feature_vals,"%lf\t%d\n", sigma, irrel_num);

        ctree_destroy(&tree);
        al_destroy(&Ys);
        al_destroy(&ytest);
    }

    fclose(train_vals);
    fclose(test_vals);
    fclose(irrel_feature_vals);

    al_destroy(&Xs);
    al_destroy(&xtest);

    free(xfile);
    free(yfile);
    free(xtestfile);
    free(ytestfile);

    // ctree_destroy(&tree);
    return EXIT_SUCCESS;
}