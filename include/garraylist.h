#ifndef ALIST_H
#define ALIST_H

#include "gallocator.h"
#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gds_types.h"
#include "gnode.h"
#include <stdbool.h>
#include <stdlib.h>
typedef ulong gnum_t;

alist_t *alist_create(size_t item_size);
int16_t  alist_init(alist_t *alist, size_t item_size);
alist_t  alist_build_num(size_t item_size, const size_t arr_size, gnum_t arr[arr_size]);
alist_t  alist_build_addr(size_t item_size, const size_t arr_size, gdata_t arr[arr_size]);
alist_t  alist_build_str(size_t arr_size, char *arr[arr_size]);
int16_t  alist_push(alist_t *list, gdata_t data);
int16_t  alist_push_safe(alist_t *list, size_t item_size, gdata_t data);
int16_t  alist_pop(alist_t *list);
int16_t  alist_set_at(alist_t *list, size_t pos, gdata_t data);
int16_t  alist_set_at_safe(alist_t *list, size_t pos, size_t item_size, gdata_t data);
int16_t  alist_rm_at(alist_t *list, size_t pos);
int16_t  alist_rm_str_at(alist_t *alist, size_t pos);
gdata_t  alist_at(alist_t *list, size_t pos);
size_t   alist_size(alist_t *list);
size_t   alist_item_size(alist_t *list);
size_t   alist_capacity(alist_t *list);
void     alist_reserve(alist_t *list, size_t size);
void     alist_set_allocator(alist_t *alist, gdata_t (*allocator_fun)(gdata_t data));
bool     alist_empty(alist_t *list);
void     alist_purge(alist_t *list);
void     alist_destroy(alist_t *list);

#ifdef __cplusplus
}
#endif // cpp
#endif // !ALIST_H
