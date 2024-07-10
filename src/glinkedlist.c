#include "../include/glinkedlist.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

list_t *list_create(size_t item_size) {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    memset(list, 0, sizeof(list_t));
    list_init(list, item_size);
    return list;
}

void list_init(list_t *list, size_t item_size) {
    list->item_size = item_size;
}

list_itr_t *list_itr_create(list_t *list) {
    list_itr_t *it = (list_itr_t *)malloc(sizeof(list_itr_t));
    if (list == NULL || it == NULL) return NULL;
    memset(it, 0, sizeof(list_itr_t));
    list_itr_init(it, list);
    return it;
}

void list_itr_init(list_itr_t *it, list_t *list) {
    it->list = list;
    it->end = list->tail;
    it->from = it->begin = list->head;
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

lnode_t *next(list_itr_t *iterator) {
    lnode_t *from = iterator->from;
    if (from == NULL) {
        if (iterator->list->head) {
            from = iterator->list->head;
        } else
            return NULL;
    }
    if (lnode_link(from) == 0) {
        return NULL;
    } else if (from == iterator->list->head) {
        iterator->next_node = lnode_link(iterator->list->head);
    } else {
        iterator->next_node = iterator->prev_node ^ lnode_link(from);
    }
    iterator->prev_node = (uintptr_t)from;
    iterator->from = (lnode_t *)iterator->next_node;
    iterator->next_node = iterator->prev_node ^ lnode_link(from);
    return iterator->from;
}

lnode_t *prev(list_itr_t *iterator) {
    lnode_t *from = iterator->from;
    if (from == NULL) {
        if (iterator->list->tail) {
            from = iterator->list->tail;
        } else
            return NULL;
    }
    if (lnode_link(from) == 0) {
        return NULL;
    } else if (from == iterator->list->tail) {
        iterator->prev_node = lnode_link(iterator->list->tail);
    } else {
        iterator->prev_node = iterator->next_node ^ lnode_link(from);
    }
    iterator->next_node = (uintptr_t)from;
    iterator->from = (lnode_t *)iterator->prev_node;
    iterator->prev_node = iterator->next_node ^ lnode_link(from);
    return iterator->from;
}

lnode_t *itr_begin(list_itr_t *iterator) {
    return iterator->begin;
}
lnode_t *itr_end(list_itr_t *iterator) {
    return iterator->end;
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

int16_t pop_front(list_t *list) {
    if (list->head == NULL || list == NULL) return EXIT_SUCCESS;
    lnode_t *old_head = list->head;
    list->head = (lnode_t *)old_head->link;
    if (list->head == NULL) {
        list->tail = NULL;
    } else
        lnode_set_link(list->head, lnode_link(list->head) ^ (uintptr_t)old_head);
    lnode_destroy(old_head);
    free(old_head);
    old_head = NULL;
    return EXIT_SUCCESS;
}

int16_t pop_back(list_t *list) {
    if (list == NULL || list->head == NULL) return EXIT_SUCCESS;
    if (list->tail == list->head) return pop_front(list);

    lnode_t *old_tail = list->tail;
    list->tail = (lnode_t *)old_tail->link;
    if (list->tail != NULL)
        lnode_set_link(list->tail, lnode_link(list->tail) ^ (uintptr_t)old_tail);

    lnode_destroy(old_tail);
    free(old_tail);
    old_tail = NULL;
    return EXIT_SUCCESS;
}

void list_clear(list_t *list) {
    while (list->head != NULL) {
        pop_front(list);
    }
}

void list_destroy(list_t *list) {
    list_clear(list);
}

void list_set_allocator(list_t *list, gdata_t (*allocator_fun)(gdata_t data)) {
    list->allocator_fun = allocator_fun;
}

list_t *itr_list(list_itr_t *iterator) {
    return iterator->list;
}

void itr_set_from(list_itr_t *iterator, lnode_t *from) {
    if (iterator == NULL || itr_list(iterator) == NULL) return;
    iterator->from = from;
}

void itr_set_begin(list_itr_t *iterator, lnode_t *begin_node) {
    iterator->begin = begin_node;
}

void itr_set_end(list_itr_t *iterator, lnode_t *end_node) {
    iterator->end = end_node;
}

int16_t dump_list(list_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL || list->head == NULL) return EXIT_FAILURE;
    lnode_t *temp = list->head;

    list_itr_t it = {0};
    list_itr_init(&it, list);

    while (temp != NULL) {
        print_data(lnode_data(temp));
        temp = next(&it);
    }
    return EXIT_SUCCESS;
}

int16_t reverse_dump_list(list_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL) return EXIT_FAILURE;
    lnode_t *temp = list->tail;

    list_itr_t *it = list_itr_create(list);
    itr_set_from(it, temp);
    if (it == NULL) return EXIT_FAILURE;
    while (temp != NULL) {
        print_data(lnode_data(temp));
        temp = prev(it);
    }
    free(it);
    return EXIT_SUCCESS;
}
