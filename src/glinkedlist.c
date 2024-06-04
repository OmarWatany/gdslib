#include "../include/glinkedlist.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

llist_t *create_list(size_t item_size) {
    llist_t *new_list = (llist_t *)malloc(sizeof(llist_t));
    memset(new_list, 0, sizeof(llist_t));
    new_list->item_size = item_size;
    return new_list;
}

list_iterator_t *create_list_iterator(llist_t *list) {
    list_iterator_t *it = (list_iterator_t *)malloc(sizeof(list_iterator_t));
    if (list == NULL || it == NULL) return NULL;
    memset(it, 0, sizeof(list_iterator_t));
    it->list = list;
    it->end = list->tail;
    it->from = it->begin = list->head;
    return it;
}

void init_list(llist_t *list, size_t item_size) {
    memset(list, 0, sizeof(llist_t));
    list->item_size = item_size;
}

void init_list_iterator(list_iterator_t *it, llist_t *list) {
    if (list == NULL || it == NULL) return;
    memset(it, 0, sizeof(list_iterator_t));
    it->list = list;
    it->end = list->tail;
    it->from = it->begin = list->head;
}

size_t list_item_size(llist_t *list) {
    return list->item_size;
}

lnode_t *list_head(llist_t *list) {
    return list->head;
}

lnode_t *list_tail(llist_t *list) {
    return list->tail;
}

lnode_t *next(list_iterator_t *iterator) {
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
    iterator->prev_node = (size_t)from;
    iterator->from = (lnode_t *)iterator->next_node;
    iterator->next_node = iterator->prev_node ^ lnode_link(from);
    return iterator->from;
}

lnode_t *prev(list_iterator_t *iterator) {
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
    iterator->next_node = (size_t)from;
    iterator->from = (lnode_t *)iterator->prev_node;
    iterator->prev_node = iterator->next_node ^ lnode_link(from);
    return iterator->from;
}

lnode_t *itr_begin(list_iterator_t *iterator) {
    return iterator->begin;
}
lnode_t *itr_end(list_iterator_t *iterator) {
    return iterator->end;
}

int16_t push_front(llist_t *list, gdata_t data) {
    lnode_t *new_node = lnode_create();
    // lnode_t *new_node = create_node();
    if (!new_node) return EXIT_FAILURE;

    gdata_t temp = NULL;
    if (list->allocator_fun) {
        // use custom allocator
        temp = list->allocator_fun(data);
    } else {
        // use default allocator
        temp = default_allocator(list->item_size, data);
    }
    lnode_set_data(new_node, temp);

    if (list->head == NULL) {
        lnode_set_link(new_node, 0);
        list->tail = new_node;
    } else {
        lnode_set_link(new_node, (size_t)list->head);
        lnode_set_link(list->head, lnode_link(list->head) ^ (size_t)new_node);
    }
    list->head = new_node;
    return EXIT_SUCCESS;
}

int16_t push_back(llist_t *list, gdata_t data) {
    lnode_t *new_node = lnode_create();
    if (!new_node) return EXIT_FAILURE;

    gdata_t temp = NULL;
    if (list->allocator_fun) {
        // use custom allocator
        temp = list->allocator_fun(data);
    } else {
        // use default allocator
        temp = default_allocator(list->item_size, data);
    }
    lnode_set_data(new_node, temp);

    if (list->tail) {
        lnode_set_link(new_node, (size_t)list->tail);
        lnode_set_link(list->tail, lnode_link(list->tail) ^ (size_t)new_node);
        list->tail = new_node;
    } else {
        push_front(list, data);
    }
    return EXIT_SUCCESS;
}

gdata_t peak_front(llist_t *list) {
    return lnode_data(list->head);
}

gdata_t peak_back(llist_t *list) {
    return lnode_data(list->tail);
}

int16_t pop_front(llist_t *list) {
    if (list->head == NULL || list == NULL) return EXIT_SUCCESS;

    list_iterator_t it = {0};
    init_list_iterator(&it, list);

    lnode_t *old_head = list->head;
    list->head = next(&it);
    if (list->head == NULL) {
        list->tail = NULL;
    } else
        lnode_set_link(list->head, lnode_link(list->head) ^ (size_t)old_head);
    lnode_destroy(old_head);
    free(old_head);
    old_head = NULL;
    return EXIT_SUCCESS;
}

int16_t pop_back(llist_t *list) {
    if (list == NULL || list->head == NULL) return EXIT_SUCCESS;
    if (list->tail == list->head) return pop_front(list);

    list_iterator_t it = {0};
    init_list_iterator(&it, list);

    itr_set_from(&it, list->tail);

    lnode_t *old_tail = list->tail;
    list->tail = prev(&it);
    if (list->tail != NULL) lnode_set_link(list->tail, lnode_link(list->tail) ^ (size_t)old_tail);

    lnode_destroy(old_tail);
    free(old_tail);
    old_tail = NULL;
    return EXIT_SUCCESS;
}

void list_destroy(llist_t *list) {
    while (list->head != NULL) {
        pop_front(list);
    }
}

void llist_set_allocator(llist_t *list, gdata_t (*allocator_fun)(gdata_t data)) {
    list->allocator_fun = allocator_fun;
}

llist_t *itr_list(list_iterator_t *iterator) {
    return iterator->list;
}

void itr_set_from(list_iterator_t *iterator, lnode_t *from) {
    if (iterator == NULL || itr_list(iterator) == NULL) return;
    iterator->from = from;
}

void itr_set_begin(list_iterator_t *iterator, lnode_t *begin_node) {
    iterator->begin = begin_node;
}

void itr_set_end(list_iterator_t *iterator, lnode_t *end_node) {
    iterator->end = end_node;
}

int16_t dump_list(llist_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL || list->head == NULL) return EXIT_FAILURE;
    lnode_t *temp = list->head;

    list_iterator_t it = {0};
    init_list_iterator(&it, list);

    while (temp != NULL) {
        print_data(lnode_data(temp));
        temp = next(&it);
    }
    return EXIT_SUCCESS;
}

int16_t reverse_dump_list(llist_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL) return EXIT_FAILURE;
    lnode_t *temp = list->tail;

    list_iterator_t *it = create_list_iterator(list);
    itr_set_from(it, temp);
    if (it == NULL) return EXIT_FAILURE;
    while (temp != NULL) {
        print_data(lnode_data(temp));
        temp = prev(it);
    }
    free(it);
    return EXIT_SUCCESS;
}
