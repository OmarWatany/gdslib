#ifndef GTYPES_H
#define GTYPES_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gallocator.h"
#include "gnode.h"
#include <stdbool.h>
#include <stdlib.h>

typedef void *gdata_t;

typedef struct {
    anode_t *buf;
    size_t   item_size, capacity, size;
    gdata_t (*allocator_fun)(gdata_t data);
} alist_t;

typedef struct {
    anode_t *buffer;
    size_t   read_pointer, write_pointer, capacity, size, item_size;
    gdata_t (*allocator_fun)(gdata_t data);
} ringbuffer_t;

typedef struct {
    alist_t buf;
} astack_t;

typedef struct {
    lnode_t *head, *tail;
    size_t   item_size;
    gdata_t (*allocator_fun)(gdata_t data);
} list_t;

typedef struct {
    list_t   *list;
    lnode_t  *from, *begin, *end;
    uintptr_t prev_node, next_node;
} list_itr_t;

typedef struct {
    list_t list;
    size_t length;
} queue_t;

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
