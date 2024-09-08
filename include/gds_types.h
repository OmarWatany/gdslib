#ifndef GTYPES_H
#define GTYPES_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gallocator.h"
#include "gnodes_types.h"
#include <stdbool.h>
#include <stdlib.h>

typedef void *gdata_t;
typedef void (*for_each_fn)(gdata_t node);
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
    tnode_t        *root;
    size_t          item_size, k;
    cmp_fun         cmp_fun;
    allocator_fun_t allocator_fun;
} ktree_t;

typedef struct {
    ktree_t       *tree;
    TRAVERSE_ORDER order;
} ktree_itr_t;

typedef struct heap_t heap_t;
typedef ktree_t       btree_t;

ktree_t *kt_create(size_t item_size, size_t k);
heap_t  *heap_create(size_t item_size, size_t k, HEAP_TYPE type);
btree_t *bt_create(size_t item_size);

typedef struct {
    anode_t        *buf;
    size_t          item_size, capacity, size;
    allocator_fun_t allocator_fun;
} alist_t;

typedef struct {
    anode_t        *buf;
    size_t          read_pointer, write_pointer, capacity, size, item_size;
    allocator_fun_t allocator_fun;
} ringbuffer_t;

typedef struct {
    alist_t buf;
} astack_t;

typedef struct {
    lnode_t        *head, *tail;
    size_t          item_size;
    allocator_fun_t allocator_fun;
} list_t;

typedef struct gitr_t gitr_t;
struct gitr_t {
    gdata_t   ds, other;
    uintptr_t begin, end, next_node, prev_node;
    gnode_t *(*next)(gitr_t *iterator);
    gnode_t *(*prev)(gitr_t *iterator);
};

typedef struct {
    list_t   *list;
    lnode_t  *from, *begin, *end;
    uintptr_t prev_node, next_node;
} list_itr_t;

typedef struct {
    list_t list;
    size_t length;
} queue_t;

struct heap_t {
    alist_t   buf;
    size_t    k;
    HEAP_TYPE type;
    cmp_fun   cmp_fun;
};

typedef struct {
    heap_t h;
    size_t item_size;
} pqueue_t;

typedef struct {
    list_t list;
    size_t length;
} deque_t;

typedef struct {
    list_t list;
} stack_t;

#ifdef __cplusplus
}
#endif // cpp
#endif // GTYPES_H
