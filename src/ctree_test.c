#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"
#include "stringappend.h"
#include "classtree.h"
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

    // print_double_vect_al(&Xs);
    // print_double_vect_al(&Ys);
    
    unsigned int num_vars = al_get_num_doubles(&Xs);
    ctree_t tree;
    ctree_setmode_samplesize(&tree, 65);
    ctree_fit(&tree, num_vars, &Xs, &Ys);
    ctree_print(tree);

    al_destroy(&Xs);
    al_destroy(&Ys);

    arraylist_t xtest, ytest;
    al_get_from_file(&xtest, xtestfile);
    al_get_from_file(&ytest, ytestfile);

    ctree_destroy(&tree);
    al_destroy(&xtest);
    al_destroy(&ytest);
    free(xfile);
    free(yfile);
    free(xtestfile);
    free(ytestfile);

    return EXIT_SUCCESS;
}
