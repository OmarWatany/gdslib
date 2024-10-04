#include "glinkedlist.h"
#include "gitr.h"
#include "gnode.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

list_t *list_create(size_t item_size) {
    list_t *list = (list_t *)calloc(1, sizeof(list_t));
    list_init(list, item_size);
    return list;
}

inline void list_init(list_t *list, size_t item_size) {
    list->item_size = item_size;
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

inline int16_t push_front(list_t *list, gdata_t data) {
    return push_front_safe(list, list->item_size, data);
}

inline int16_t push_back(list_t *list, gdata_t data) {
    return push_back_safe(list, list->item_size, data);
}

inline gdata_t peak_front(list_t *list) {
    return lnode_data(list->head);
}

inline gdata_t peak_back(list_t *list) {
    return lnode_data(list->tail);
}

int16_t pop_front_s(list_t *list, size_t size, void **buffer, bool str) {
    if (list == NULL || list->head == NULL) return EXIT_FAILURE;
    lnode_t *old_head = list->head;
    if (buffer) {
        if (str) {
            *buffer = old_head->data;
        } else {
            if (*buffer) memcpy(*buffer, old_head->data, size);
        }
    }
    list->head = (lnode_t *)old_head->link;
    if (list->head == NULL) {
        list->tail = NULL;
    } else
        lnode_set_link(list->head, lnode_link(list->head) ^ (uintptr_t)old_head);

    if (!(buffer && str)) lnode_destroy(old_head);
    free(old_head);
    return EXIT_SUCCESS;
}

int16_t pop_back_s(list_t *list, size_t size, void **buffer, bool str) {
    if (list == NULL || list->head == NULL) return EXIT_FAILURE;
    lnode_t *old_tail = list->tail;
    if (buffer) {
        if (str)
            // pop front str
            *buffer = old_tail->data;
        else {
            // pop front
            if (*buffer) memcpy(*buffer, old_tail->data, size);
        }
    }
    // both
    list->tail = (lnode_t *)old_tail->link;
    if (list->tail == NULL) {
        list->tail = NULL;
    } else
        lnode_set_link(list->tail, lnode_link(list->tail) ^ (uintptr_t)old_tail);

    if (!(buffer && str)) lnode_destroy(old_tail);

    // both
    free(old_tail);
    return EXIT_SUCCESS;
}

inline int16_t pop_front(list_t *list, void *buffer) {
    return pop_front_s(list, list->item_size, &buffer, false);
}

inline int16_t pop_back(list_t *list, void *buffer) {
    return pop_back_s(list, list->item_size, buffer, false);
}

char *strpop_front(list_t *list) {
    if (list == NULL || list->head == NULL) return NULL;
    void *temp = NULL;
    return !pop_front_s(list, 0, &temp, true) ? (char *)temp : NULL;
}

char *strpop_back(list_t *list) {
    if (list == NULL || list->head == NULL) return NULL;
    void *temp = NULL;
    return !pop_back_s(list, 0, &temp, true) ? temp : NULL;
}

inline void list_purge(list_t *list) {
    while (list->head != NULL)
        pop_front_s(list, list->item_size, NULL, false);
}

void list_destroy(list_t *list) {
    list_purge(list);
}

inline void list_set_allocator(list_t *list, gdata_t (*allocator_fun)(gdata_t data)) {
    list->allocator_fun = allocator_fun;
}

int16_t dump_list(list_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL || list->head == NULL) return EXIT_FAILURE;
    gdata_t temp = list->head->data;
    gitr_t  itr = list_gitr(list);

    while (temp != NULL) {
        print_data(temp);
        temp = itr_next(&itr);
    }
    gitr_destroy(&itr);
    return EXIT_SUCCESS;
}

int16_t reverse_dump_list(list_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL) return EXIT_FAILURE;
    gitr_t   itr = list_gitr(list);
    lnode_t *temp = itr_end(&itr);
    gitr_set_from(&itr, temp);
    do {
        temp = itr_prev(&itr);
        print_data(temp);
    } while (temp != itr_begin(&itr));
    gitr_destroy(&itr);
    return EXIT_SUCCESS;
}
