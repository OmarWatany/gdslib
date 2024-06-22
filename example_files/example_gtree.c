#include "../include/gtree.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

static int gcmp_int(gdata_t data1, gdata_t data2) {
    if (!(data1 && data2)) return 0;
    int i = *(int *)data1, j = *(int *)data2;
    return i > j ? 1 : i < j ? -1 : 0;
}

static void inc(tnode_t *node, size_t lvl) {
    *(int *)tnode_data(node) += lvl + 3;
}

static void iprintLvl(tnode_t *node, size_t lvl) {
    if (!node) return;
    printf("%d - lvl %ld\n", *(int *)tnode_data(node), lvl);
}

static void iprintTree(tnode_t *node, size_t lvl) {
    printf(" ");
    for (size_t i = 0; i < lvl; i++)
        printf("    ");
    printf("%d\n", *(int *)tnode_data(node));
}

int main() {
    btree_t tree = {0};
    btr_init(&tree, sizeof(int), gcmp_int);

    // int arr[] = {197, 198, 32,  65,  267, 105, 345, 319, 245, 45, 386,
    //              47,  43,  195, 209, 389, 384, 259, 89,  29,  9};
    // int arr[] = {197, 198, 32,  65,  267, 105, 319, 245, 45, 386, 47,
    //              43,  195, 209, 389, 384, 259, 89,  29,  9,  345};
    int arr[] = {197, 198, 32,  65,  267, 105, 319, 245, 45, 386, 47,
                 43,  195, 209, 389, 345, 384, 259, 89,  29, 9};

    for (size_t i = 0; i < 21; i++) {
        bst_add(&tree, &arr[i]);
    }

    srand(time(0));
    int j = 0;
    for (size_t i = 0; i < 2000; i++) {
        j = rand() % (1500) + 200;
        bst_add(&tree, &j);
    }

    printf("MAX : %d \n", *(int *)bst_max(&tree));
    printf("MIN : %d \n", *(int *)bst_min(&tree));

    int d = 10000;
    bst_add(&tree, &d);

    // kt_for_each(&tree, IN_ORDER, iprintTree);
    // printf("__ BREADTH START __\n");
    // kt_for_each(&tree, BREADTH_FIRST_ORDER, iprintLvl);
    // printf("__ BREADTH END __\n");

    printf("tree size : %ld\n", tree.size);
    size_t lvl = 4, lvlc = pow(tree.k, lvl);

    tnode_t **childs = kt_grand_childrens(&tree, lvl);
    printf("lvl: %ld , children count %ld \n", lvl, lvlc);

    d = 345;
    if (bst_find(&tree, &d)) {
        bst_delete(&tree, &d);
        kt_for_each(&tree, IN_ORDER, iprintTree);
    }
    // } else {
    //     for (size_t j = 0; j < lvlc && childs; j++) {
    //         if (childs[j])
    //             printf("child n : %ld lvl: %ld = %d\n", j, lvl, *(int *)tnode_data(childs[j]));
    //     }
    //     printf("size: %ld \n", tree.size);
    // }
    //
    // kt_for_each(&tree, IN_ORDER, inc);
    // kt_for_each(&tree, IN_ORDER, iprintLvl);
    printf("BREADTH FIRST PRINT\n");
    kt_for_each(&tree, BREADTH_FIRST_ORDER, iprintLvl);

    free(childs);
    bst_destroy(&tree);

    return 0;
}
