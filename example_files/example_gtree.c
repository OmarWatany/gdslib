#include "gds_types.h"
#include "gitr.h"
#include "gtree.h"
#include <stdio.h>

static int gcmp_int(gdata_t data1, gdata_t data2) {
    if (!(data1 && data2)) return 0;
    int i = *(int *)data1, j = *(int *)data2;
    return i > j ? 1 : i < j ? -1 : 0;
}

static void inc(gdata_t for_data) {
    if (!for_data) return;
    tree_for_data *d = (tree_for_data *)for_data;
    *(int *)tnode_data(d->node) += d->lvl + 3;
}

static void iprint(gdata_t for_data) {
    if (!for_data) return;
    tree_for_data *d = (tree_for_data *)for_data;
    printf("%d - ", *(int *)gnode_data(d->node));
}

static void heap_iprint(gdata_t for_data) {
    iprint(for_data);
}

static void iprintLvl(gdata_t for_data) {
    if (!for_data) return;
    tree_for_data d = *(tree_for_data *)for_data;
    if (!d.node) return;
    printf("%d - lvl %ld\n", *(int *)gnode_data(d.node), d.lvl);
}

static void iprintTree(gdata_t for_data) {
    tree_for_data d = *(tree_for_data *)for_data;
    printf(" ");
    for (size_t i = 0; i < d.lvl; i++)
        printf("    ");
    printf("%d\n", *(int *)gnode_data(d.node));
}

void random_output() {
    btree_t tree = {0};
    bt_init(&tree, sizeof(int), gcmp_int);
    // test 345 in various conditions such as (parent to tree ,parent to one child,leaf)
    // int arr[] = {197, 198, 32,  65,  267, 105, 345, 319, 245, 45, 386,
    //              47,  43,  195, 209, 389, 384, 259, 89,  29,  9};
    // int arr[] = {197, 198, 32,  65,  267, 105, 319, 245, 45, 386, 47,
    //              43,  195, 209, 389, 384, 259, 89,  29,  9,  345};
    int arr[] = {197, 198, 32,  65,  267, 105, 319, 245, 45, 386, 47,
                 43,  195, 209, 389, 345, 384, 259, 89,  29, 9};

    for (size_t i = 0; i < 21; i++) {
        bst_add(&tree, &arr[i]);
    }
    printf("MAX  : %d \n", *(int *)bst_max(&tree));
    printf("MIN  : %d \n", *(int *)bst_min(&tree));
    printf("SIZE : %ld \n", tree.size);

    /* srand(time(0)); */
    /* int j = 0; */
    /* for (size_t i = 0; i < 2000; i++) { */
    /*     j = rand() % (1500) + 200; */
    /*     bst_add(&tree, &j); */
    /* } */

    int d = 10000;
    bst_add(&tree, &d);
    printf("MAX  : %d \n", *(int *)bst_max(&tree));
    printf("MIN  : %d \n", *(int *)bst_min(&tree));
    printf("SIZE : %ld \n", tree.size);

    // kt_for_each(&tree, IN_ORDER, iprintTree);
    // printf("__ BREADTH START __\n");
    // kt_for_each(&tree, BREADTH_FIRST_ORDER, iprintLvl);
    // printf("__ BREADTH END __\n");

    /* size_t    lvl = 4, lvlc = pow(tree.k, lvl); */
    /* tnode_t **childs = kt_grand_childrens(&tree, lvl); */
    /* printf("lvl: %ld , children count %ld \n", lvl, lvlc); */

    d = 345;
    if (bst_find(&tree, &d)) {
        bst_delete(&tree, &d);
        kt_for_each(&tree, IN_ORDER, iprintTree);
    }

    printf("MAX  : %d \n", *(int *)bst_max(&tree));
    printf("MIN  : %d \n", *(int *)bst_min(&tree));
    printf("SIZE : %ld \n", tree.size);

    //  else {
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

    printf("------------------- \n");
    printf("Tree iterator\n");
    printf("SIZE : %ld \n", tree.size);
    gitr_t   itr = tr_gitr_o(&tree, POST_ORDER);
    gnode_t *temp = itr_begin(&itr);

    size_t idx = 0;
    do {
        printf("%3ld %5d - \n", ++idx, *(int *)gnode_data(temp));
    } while ((temp = itr_next(&itr)));

    while (temp != itr_begin(&itr)) {
        temp = itr_prev(&itr);
        printf("%3ld %5d - \n", --idx, *(int *)gnode_data(temp));
    }

    gitr_destroy(&itr);

    /* free(childs); */
    bst_destroy(&tree);
}

void test_kheap(size_t k) {
    printf("K ->  %zu\n", k);
    // int arr[] = {11, 2, 5, 3, 10, 15, 13};
    int arr[] = {23, 7, 92, 6, 12, 14, 40, 44, 20, 21, 92};
    // int arr[] = {92, 44, 40, 23, 21, 12, 14, 6, 20, 7};
    // int arr[] = {3, 5, 20, 10, 15, 30, 50};
    int arr_size = sizeof(arr) / sizeof(arr[0]);
    // should print perfect tree and BREADTH print should print it
    heap_t hp = {0};
    heap_init(&hp, sizeof(int), k, MAX_HEAP);
    heap_set_cmp_fun(&hp, gcmp_int);

    for (int i = 0; i < arr_size; i++) {
        heap_add(&hp, &arr[i]);
    }

    if (valid_heap(&hp, 0))
        printf("VALID HEAP\n");
    else
        printf("UNVALID HEAP\n");

    printf("BREADTH FIRST PRINT\n");
    heap_for_each(&hp, heap_iprint);
    printf("\n");
    // heap_for_each(&hp, iprint);
    // printf("\n");
    heap_for_each_order(&hp, IN_ORDER, iprintTree);
    printf("---------------\n");

    heap_pop(&hp);
    heap_for_each(&hp, heap_iprint);
    printf("\n");
    printf("---------------\n");

    int i = 0x45;
    heap_add(&hp, &i);
    heap_add(&hp, &i);

    if (valid_heap(&hp, 0))
        printf("VALID HEAP\n");
    else
        printf("UNVALID HEAP\n");
    heap_for_each(&hp, heap_iprint);
    printf("\n");

    heap_for_each_order(&hp, IN_ORDER, iprintTree);
    printf("\n");

    heap_destroy(&hp);
    printf("---------------\n");
    printf("\n");
}

int main() {
    random_output();
    /* test_kheap(2); */
    // test_kheap(4);

    return 0;
}
