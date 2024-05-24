#include "../include/gqueue.h"
#include <stdio.h>
#include <stdlib.h>

struct queue_t {
    llist_t *list;
    size_t   length;
};

queue_t *create_queue(size_t item_size) {
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    if (queue == NULL)
        return NULL;
    queue->list = create_list(item_size);
    if (queue->list == NULL)
        return NULL;
    queue->length = 0;
    return queue;
}

int16_t enqueue(queue_t *queue, gdata_t data) {
    if (queue == NULL || queue->list == NULL)
        return EXIT_FAILURE;
    push_front(queue->list, data);
    queue->length++;
    return EXIT_SUCCESS;
}

int16_t dequeue(queue_t *queue) {
    if (queue == NULL || queue->list == NULL)
        return EXIT_FAILURE;
    gdata_t temp = peak_back(queue->list);
    if (temp != NULL)
        pop_back(queue->list);
    queue->length--;
    return EXIT_SUCCESS;
}

size_t queue_length(queue_t *queue) {
    if (queue == NULL || queue->list == NULL)
        return 0;
    return queue->length;
}

gdata_t queue_front(queue_t *queue) {
    if (queue == NULL || queue->list == NULL)
        return NULL;
    return peak_back(queue->list);
}

gdata_t queue_back(queue_t *queue) {
    if (queue == NULL || queue->list == NULL)
        return NULL;
    return peak_front(queue->list);
}

bool queue_is_empty(queue_t *queue) {
    if (queue == NULL || queue->length == 0 || queue->list == NULL)
        return true;
    return false;
}

bool in_queue(queue_t *heystack, gdata_t needle, bool (*search_fun)(node_t *node, gdata_t data)) {
    if (queue_is_empty(heystack))
        return false;

    list_iterator_t *itr = create_list_iterator(heystack->list);
    itr_set_begin(itr, list_head(heystack->list));
    node_t *temp = itr_begin(itr);

    bool r = false;
    while (temp != NULL && !r) {
        if (search_fun(temp, needle))
            r = true;
        temp = next(itr);
    }
    free(itr);
    return r;
}

void dump_queue(queue_t *queue, void (*_print_data)(gdata_t data)) {
    reverse_dump_list(queue->list, _print_data);
    printf("\n");
}

void destroy_queue(queue_t **queue) {
    if (*queue == NULL || (*queue)->list == NULL)
        return;
    destroy_list(&(*queue)->list);
    free(*queue);
    *queue = NULL;
}
