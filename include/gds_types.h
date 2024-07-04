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

typedef struct Private Private;

typedef struct {
    size_t   capacity, size;
    Private *pv;
} alist_t;

typedef struct {
    Private *pv;
} ringbuffer_t;

typedef struct {
    Private *pv;
} astack_t;

typedef struct {
    Private *pv;
} llist_t;

typedef struct {
    llist_t *list;
    lnode_t *from, *begin, *end;
    size_t   prev_node, next_node;
} list_itr_t;

typedef struct {
    llist_t list;
    size_t  length;
} queue_t;

typedef struct {
    llist_t list;
    size_t  length;
} deque_t;

typedef struct {
    llist_t list;
} stack_t;

#ifdef __cplusplus
}
#endif // cpp
#endif // GTYPES_H
