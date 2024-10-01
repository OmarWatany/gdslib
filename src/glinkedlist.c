#include "glinkedlist.h"
#include "gds_types.h"
#include "gitr.h"
#include <alloca.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

list_t *list_create(size_t item_size) {
    list_t *list = (list_t *)calloc(1, sizeof(list_t));
    list_init(list, item_size);
    return list;
}

void list_init(list_t *list, size_t item_size) {
    list->item_size = item_size;
}

size_t list_item_size(list_t *list) {
    return list->item_size;
}

lnode_t *list_head(list_t *list) {
    return list->head;
}

lnode_t *list_tail(list_t *list) {
    return list->tail;
}

int16_t push_front_safe(list_t *list, size_t item_size, gdata_t data) {
    lnode_t *new_node = lnode_create();
    if (!new_node) return EXIT_FAILURE;

    gdata_t allocated = NULL;
    if (list->allocator_fun) {
        // use custom allocator
        allocated = list->allocator_fun(data);
    } else {
        // use default allocator
        allocated = default_allocator(item_size, data);
    }
    lnode_set_data(new_node, allocated);

    if (list->head == NULL) {
        lnode_set_link(new_node, 0);
        list->tail = new_node;
    } else {
        lnode_set_link(new_node, (uintptr_t)list->head);
        lnode_set_link(list->head, lnode_link(list->head) ^ (uintptr_t)new_node);
    }
    list->head = new_node;
    return EXIT_SUCCESS;
}

int16_t push_back_safe(list_t *list, size_t item_size, gdata_t data) {
    lnode_t *new_node = lnode_create();
    if (!new_node) return EXIT_FAILURE;

    gdata_t allocated = NULL;
    if (list->allocator_fun) {
        // use custom allocator
        allocated = list->allocator_fun(data);
    } else {
        // use default allocator
        allocated = default_allocator(item_size, data);
    }
    lnode_set_data(new_node, allocated);

    if (list->tail) {
        lnode_set_link(new_node, (uintptr_t)list->tail);
        lnode_set_link(list->tail, lnode_link(list->tail) ^ (uintptr_t)new_node);
        list->tail = new_node;
    } else {
        push_front(list, data);
    }
    return EXIT_SUCCESS;
}

int16_t push_front(list_t *list, gdata_t data) {
    return push_front_safe(list, list->item_size, data);
}

int16_t push_back(list_t *list, gdata_t data) {
    return push_back_safe(list, list->item_size, data);
}

gdata_t peak_front(list_t *list) {
    return lnode_data(list->head);
}

gdata_t peak_back(list_t *list) {
    return lnode_data(list->tail);
}

gdata_t pop_front_s(list_t *list, size_t size, bool return_value, bool str) {
    lnode_t *old_head = list->head;
    gdata_t  r = return_value ? (str ? old_head->data
                                     : memcpy(memset(alloca(size), 0, size), old_head->data, size))
                              : NULL;

    list->head = (lnode_t *)old_head->link;
    if (list->head == NULL) {
        list->tail = NULL;
    } else
        lnode_set_link(list->head, lnode_link(list->head) ^ (uintptr_t)old_head);

    if (!str) lnode_destroy(old_head);
    free(old_head);
    old_head = NULL;
    return r;
}

gdata_t pop_front(list_t *list) {
    if (list->head == NULL || list == NULL) return NULL;
    return pop_front_s(list, list->item_size, true, false);
}

char *spop_front(list_t *list, size_t *size) {
    if (list == NULL || list->head == NULL) return NULL;
    size_t tsize = strlen((char *)list->head->data) + 1;
    if (size) *size = tsize;
    return pop_front_s(list, tsize, true, true);
}

gdata_t pop_back_s(list_t *list, size_t size, bool return_value, bool str) {
    if (list->tail == list->head) return pop_front_s(list, size, true, false);
    lnode_t *old_tail = list->tail;
    gdata_t  r = return_value ? (str ? old_tail->data
                                     : memcpy(memset(alloca(size), 0, size), old_tail->data, size))
                              : NULL;
    list->tail = (lnode_t *)old_tail->link;
    if (list->tail != NULL)
        lnode_set_link(list->tail, lnode_link(list->tail) ^ (uintptr_t)old_tail);

    if (!str) lnode_destroy(old_tail);
    free(old_tail);
    old_tail = NULL;
    return r;
}

gdata_t pop_back(list_t *list) {
    if (list == NULL || list->head == NULL) return NULL;
    return pop_back_s(list, list->item_size, true, false);
}
char *spop_back(list_t *list, size_t *size) {
    if (list == NULL || list->head == NULL) return NULL;
    size_t tsize = strlen((char *)list->tail->data);
    if (size) *size = tsize;
    return pop_back_s(list, tsize, true, true);
}

void list_purge(list_t *list) {
    while (list->head != NULL)
        pop_front_s(list, list->item_size, false, false);
}

void list_destroy(list_t *list) {
    list_purge(list);
}

void list_set_allocator(list_t *list, gdata_t (*allocator_fun)(gdata_t data)) {
    list->allocator_fun = allocator_fun;
}

int16_t dump_list(list_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL || list->head == NULL) return EXIT_FAILURE;
    lnode_t *temp = list->head;
    gitr_t   itr = list_gitr(list);

    while (temp != NULL) {
        print_data(lnode_data(temp));
        temp = gitr_next(&itr);
    }
    gitr_destroy(&itr);
    return EXIT_SUCCESS;
}

int16_t reverse_dump_list(list_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL) return EXIT_FAILURE;
    lnode_t *temp = list->tail;
    gitr_t   itr = list_gitr(list);
    gitr_set_from(&itr, temp);
    while (temp != NULL) {
        print_data(lnode_data(temp));
        temp = gitr_prev(&itr);
    }
    gitr_destroy(&itr);
    return EXIT_SUCCESS;
}
