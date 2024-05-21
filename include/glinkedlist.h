#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include <stdlib.h>

typedef void                  *gdata_t;
typedef struct node_t          node_t;
typedef struct llist_t         llist_t;
typedef struct list_iterator_t list_iterator_t;

llist_t         *create_list(void (*allocate_data)(node_t *, gdata_t data));
list_iterator_t *create_list_iterator(llist_t *list);

node_t *list_head(llist_t *list);
node_t *list_tail(llist_t *list);
node_t *next(list_iterator_t *iterator);
node_t *prev(list_iterator_t *iterator);
node_t *itr_begin(list_iterator_t *iterator);
node_t *itr_end(list_iterator_t *iterator);

int16_t push_front(llist_t *list, gdata_t data);
int16_t push_back(llist_t *list, gdata_t data);
int16_t pop_front(llist_t *list);
int16_t pop_back(llist_t *list);

void    node_set_data(node_t *node, gdata_t data);
gdata_t node_data(node_t *node);
gdata_t peak_front(llist_t *list);
gdata_t peak_back(llist_t *list);
void    clear_list(llist_t *list);
void    destroy_list(llist_t **list);
void    itr_set_from(list_iterator_t *iterator, node_t *from);
void    itr_set_begin(list_iterator_t *iterator, node_t *begin_node);
void    itr_set_end(list_iterator_t *iterator, node_t *end_node);

llist_t *itr_list(list_iterator_t *iterator);

int16_t dump_list(llist_t *list, void (*print_data)(gdata_t));
int16_t reverse_dump_list(llist_t *list, void (*print_data)(gdata_t));

#ifdef __cplusplus
}
#endif // cpp
#endif // LINKED_LIST_H
