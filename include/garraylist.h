#ifndef ALIST_H
#define ALIST_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gallocator.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct alist_t alist_t;
typedef struct anode_t anode_t;

struct alist_t {
    anode_t *buf;
    size_t   item_size, capacity, size;
    gdata_t (*allocator_fun)(gdata_t data);
};

alist_t *create_alist(size_t item_size);
int16_t  alist_push(alist_t *list, gdata_t data);
int16_t  alist_pop(alist_t *list);
int16_t  alist_set_at(alist_t *list, size_t pos, gdata_t data);
int16_t  alist_rm_at(alist_t *list, size_t pos);
gdata_t  alist_at(alist_t *list, size_t pos);
size_t   alist_size(alist_t *list);
size_t   alist_capacity(alist_t *list);
void     alist_reserve(alist_t *list, size_t size);
void     alist_set_allocator(alist_t *alist, gdata_t (*allocator_fun)(gdata_t data));
bool     alist_empty(alist_t *list);

void clear_alist(alist_t *list);
void destroy_alist(alist_t **list);

#ifdef __cplusplus
}
#endif // cpp
#endif // !ALIST_H
