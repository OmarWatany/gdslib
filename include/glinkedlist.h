#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gds_types.h"
#include <stdlib.h>

list_t     *list_create(size_t item_size);
list_itr_t *list_itr_create(list_t *list);

void list_init(list_t *list, size_t item_size);
void list_itr_init(list_itr_t *it, list_t *list);

size_t   list_item_size(list_t *list);
lnode_t *list_head(list_t *list);
lnode_t *list_tail(list_t *list);
lnode_t *next(list_itr_t *iterator);
lnode_t *prev(list_itr_t *iterator);
lnode_t *itr_begin(list_itr_t *iterator);
lnode_t *itr_end(list_itr_t *iterator);

int16_t push_front_safe(list_t *list, size_t item_size, gdata_t data);
int16_t push_back_safe(list_t *list, size_t item_size, gdata_t data);
int16_t push_front(list_t *list, gdata_t data);
int16_t push_back(list_t *list, gdata_t data);
int16_t pop_front(list_t *list);
int16_t pop_back(list_t *list);
gdata_t peak_front(list_t *list);
gdata_t peak_back(list_t *list);

void list_clear(list_t *list);
void list_destroy(list_t *list);
void list_set_allocator(list_t *list, gdata_t (*allocator_fun)(gdata_t data));
void itr_set_from(list_itr_t *iterator, lnode_t *from);
void itr_set_begin(list_itr_t *iterator, lnode_t *begin_node);
void itr_set_end(list_itr_t *iterator, lnode_t *end_node);

list_t *itr_list(list_itr_t *iterator);

int16_t dump_list(list_t *list, void (*print_data)(gdata_t));
int16_t reverse_dump_list(list_t *list, void (*print_data)(gdata_t));

#ifdef __cplusplus
}
#endif // cpp
#endif // LINKED_LIST_H
