#ifndef GTREE_T
#define GTREE_T

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gallocator.h"
#include "gnode.h"
#include "gnodes_types.h"
#include <stdbool.h>

typedef void (*for_each_fn)(tnode_t *node, size_t level);
typedef int (*cmp_fun)(gdata_t data1, gdata_t data2); // data1 > data2 = 1

typedef enum {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER,
    BREADTH_FIRST_ORDER,
} TRAVERSE_ORDER;

typedef enum {
    MIN_HEAP,
    MAX_HEAP,
} HEAP_TYPE;

typedef struct {
    tnode_t *root;
    size_t   item_size, k;
    int (*cmp_fun)(gdata_t data1, gdata_t data2); // data1 > data2 = 1
    allocator_fun_t allocator_fun;
} ktree_t;

typedef struct {
    ktree_t   in;
    tnode_t  *lastParent;
    HEAP_TYPE type;
} heap_t;

typedef struct {
    ktree_t       *tree;
    TRAVERSE_ORDER order;
} ktree_itr_t;

typedef ktree_t btree_t;

ktree_t *kt_create(size_t item_size, size_t k);
heap_t  *heap_create(size_t item_size, size_t k, HEAP_TYPE type);
btree_t *bt_create(size_t item_size);

void kt_init(ktree_t *tree, size_t item_size, size_t k);
void kt_set_allocator(ktree_t *tree, allocator_fun_t allocator_fun);
void kt_add_safe(ktree_t *tree, gdata_t data); // TODO: implement
void kt_add(ktree_t *tree, gdata_t data);
void kt_destroy(ktree_t *tree);
void kt_for_each(ktree_t *tree, TRAVERSE_ORDER order, for_each_fn function);

void    heap_init(heap_t *heap, size_t item_size, size_t k, HEAP_TYPE type);
void    heap_set_allocator(heap_t *heap, allocator_fun_t allocator_fun);
void    heap_set_cmp_fun(heap_t *heap, cmp_fun cmp);
void    heap_add_safe(heap_t *heap, size_t item_size, gdata_t data);
void    heap_add(heap_t *heap, gdata_t data);
void    heap_destroy(heap_t *heap);
void    heap_for_each(heap_t *heap, TRAVERSE_ORDER order, for_each_fn function);
void    heap_pop(heap_t *heap);
gdata_t heap_peak(heap_t *heap);
size_t  heap_height(heap_t *heap);

void      heapify(heap_t *heap, tnode_t *root);
tnode_t **kt_grand_childrens(ktree_t *tree, size_t lvl);

void bt_init(btree_t *tree, size_t item_size, cmp_fun cmp);
void bst_add(btree_t *tree, gdata_t data);
bool bst_find(btree_t *heystack, gdata_t needle);
void bst_delete(btree_t *tree, gdata_t data);

gdata_t *bst_min(ktree_t *tree);
gdata_t *bst_max(ktree_t *tree);

void bst_destroy(btree_t *tree);

#ifdef __cplusplus
}
#endif // cpp
#endif // GTREE_T
