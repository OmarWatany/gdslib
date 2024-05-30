#include "../include/glinkedlist.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    gdata_t data;
    size_t  item_size;
} allocator_data_t;

struct llist_t {
    node_t *head, *tail;
    size_t  item_size;
    gdata_t (*allocator_fun)(gdata_t data);
};

struct list_iterator_t {
    llist_t *list;
    node_t  *from, *begin, *end;
    size_t   prev_node, next_node;
};

gdata_t default_allocator(gdata_t allocator_data);

llist_t *create_list(size_t item_size) {
    llist_t *new_list   = (llist_t *)malloc(sizeof(llist_t));
    new_list->head      = NULL;
    new_list->tail      = NULL;
    new_list->item_size = item_size;

    new_list->allocator_fun = NULL;
    return new_list;
}

list_iterator_t *create_list_iterator(llist_t *list) {
    list_iterator_t *it = (list_iterator_t *)malloc(sizeof(list_iterator_t));
    if (list == NULL || it == NULL)
        return NULL;
    it->list = list;
    it->end  = list->tail;
    it->from = it->begin = list->head;
    it->next_node = it->prev_node = 0;
    return it;
}

size_t list_item_size(llist_t *list) {
    return list->item_size;
}

node_t *list_head(llist_t *list) {
    return list->head;
}

node_t *list_tail(llist_t *list) {
    return list->tail;
}

node_t *next(list_iterator_t *iterator) {
    node_t *from = iterator->from;
    if (from == NULL) {
        if (iterator->list->head) {
            from = iterator->list->head;
        } else
            return NULL;
    }
    if (node_link(from) == 0) {
        return NULL;
    } else if (from == iterator->list->head) {
        iterator->next_node = node_link(iterator->list->head);
    } else {
        iterator->next_node = iterator->prev_node ^ node_link(from);
    }
    iterator->prev_node = (size_t)from;
    iterator->from      = (node_t *)iterator->next_node;
    iterator->next_node = iterator->prev_node ^ node_link(from);
    return iterator->from;
}

node_t *prev(list_iterator_t *iterator) {
    node_t *from = iterator->from;
    if (from == NULL) {
        if (iterator->list->tail) {
            from = iterator->list->tail;
        } else
            return NULL;
    }
    if (node_link(from) == 0) {
        return NULL;
    } else if (from == iterator->list->tail) {
        iterator->prev_node = node_link(iterator->list->tail);
    } else {
        iterator->prev_node = iterator->next_node ^ node_link(from);
    }
    iterator->next_node = (size_t)from;
    iterator->from      = (node_t *)iterator->prev_node;
    iterator->prev_node = iterator->next_node ^ node_link(from);
    return iterator->from;
}

node_t *itr_begin(list_iterator_t *iterator) {
    return iterator->begin;
}
node_t *itr_end(list_iterator_t *iterator) {
    return iterator->end;
}

int16_t push_front(llist_t *list, gdata_t data) {
    node_t *new_node = create_node();
    if (!new_node)
        return EXIT_FAILURE;

    gdata_t temp = NULL;
    if (list->allocator_fun) {
        // use custom allocator
        temp = list->allocator_fun(data);
    } else {
        // use default allocator
        allocator_data_t t = {
            data,
            list->item_size,
        };
        temp = default_allocator(&t);
    }
    node_set_data(new_node, temp);

    if (list->head == NULL) {
        node_set_link(new_node, 0);
        list->tail = new_node;
    } else {
        node_set_link(new_node, (size_t)list->head);
        node_set_link(list->head, node_link(list->head) ^ (size_t)new_node);
    }
    list->head = new_node;
    return EXIT_SUCCESS;
}

int16_t push_back(llist_t *list, gdata_t data) {
    node_t *new_node = create_node();
    if (!new_node)
        return EXIT_FAILURE;

    gdata_t temp = NULL;
    if (list->allocator_fun) {
        // use custom allocator
        temp = list->allocator_fun(data);
    } else {
        // use default allocator
        allocator_data_t t = {
            data,
            list->item_size,
        };
        temp = default_allocator(&t);
    }
    node_set_data(new_node, temp);

    if (list->tail) {
        node_set_link(new_node, (size_t)list->tail);
        node_set_link(list->tail, node_link(list->tail) ^ (size_t)new_node);
        list->tail = new_node;
    } else {
        push_front(list, data);
    }
    return EXIT_SUCCESS;
}

gdata_t peak_front(llist_t *list) {
    return node_data(list->head);
}

gdata_t peak_back(llist_t *list) {
    return node_data(list->tail);
}

int16_t pop_front(llist_t *list) {
    if (list->head == NULL || list == NULL)
        return EXIT_SUCCESS;

    list_iterator_t *it = create_list_iterator(list);
    if (it == NULL)
        return EXIT_FAILURE;

    node_t *old_head = list->head;
    list->head       = next(it);
    if (list->head == NULL) {
        list->tail = NULL;
    } else
        node_set_link(list->head, node_link(list->head) ^ (size_t)old_head);
    destroy_node(&old_head);
    free(it);
    return EXIT_SUCCESS;
}

int16_t pop_back(llist_t *list) {

    if (list == NULL || list->head == NULL)
        return EXIT_SUCCESS;

    if (list->tail == list->head)
        return pop_front(list);

    list_iterator_t *it = create_list_iterator(list);
    if (it == NULL)
        return EXIT_FAILURE;

    itr_set_from(it, list->tail);

    node_t *old_tail = list->tail;
    list->tail       = prev(it);
    if (list->tail != NULL)
        node_set_link(list->tail, node_link(list->tail) ^ (size_t)old_tail);

    destroy_node(&old_tail);
    free(it);
    return EXIT_SUCCESS;
}

void clear_list(llist_t *list) {
    list_iterator_t *it = create_list_iterator(list);
    if (it == NULL)
        return;
    while (list->head != NULL) {
        pop_front(list);
    }
    free(it);
}

void destroy_list(llist_t **list) {
    clear_list(*list);
    free(*list);
    *list = NULL;
}

void alist_set_allocator(llist_t *list, gdata_t (*allocator_fun)(gdata_t data)) {
    list->allocator_fun = allocator_fun;
}

llist_t *itr_list(list_iterator_t *iterator) {
    return iterator->list;
}

void itr_set_from(list_iterator_t *iterator, node_t *from) {
    if (iterator == NULL || itr_list(iterator) == NULL)
        return;
    iterator->from = from;
}

void itr_set_begin(list_iterator_t *iterator, node_t *begin_node) {
    iterator->begin = begin_node;
}

void itr_set_end(list_iterator_t *iterator, node_t *end_node) {
    iterator->end = end_node;
}

int16_t dump_list(llist_t *list, void (*print_data)(gdata_t)) {
    if (list == NULL || list->head == NULL)
        return EXIT_FAILURE;

    node_t *temp = list->head;

    list_iterator_t *it = create_list_iterator(list);
    if (it == NULL)
        return EXIT_FAILURE;

    while (temp != NULL) {
        print_data(node_data(temp));
        temp = next(it);
    }
    free(it);
    return EXIT_SUCCESS;
}

int16_t reverse_dump_list(llist_t *list, void (*print_data)(gdata_t)) {
    node_t *temp = list->tail;

    if (list == NULL)
        return EXIT_FAILURE;

    list_iterator_t *it = create_list_iterator(list);
    itr_set_from(it, temp);
    if (it == NULL)
        return EXIT_FAILURE;
    while (temp != NULL) {
        print_data(node_data(temp));
        temp = prev(it);
    }
    free(it);
    return EXIT_SUCCESS;
}

gdata_t default_allocator(gdata_t allocator_data) {
    size_t  item_size = ((allocator_data_t *)allocator_data)->item_size;
    gdata_t data      = ((allocator_data_t *)allocator_data)->data;

    gdata_t *temp = malloc(item_size);
    return memcpy(temp, data, item_size);
}
