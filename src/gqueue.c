#include "../include/gqueue.h"
#include "../include/glinkedlist.h"
#include <stdio.h>
#include <stdlib.h>

queue_t *queue_create(size_t item_size) {
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    if (queue == NULL) return NULL;
    list_init(&queue->list, item_size);
    queue->length = 0;
    return queue;
}

void queue_init(queue_t *queue, size_t item_size) {
    list_init(&queue->list, item_size);
    queue->length = 0;
}

int16_t enqueue(queue_t *queue, gdata_t data) {
    if (queue == NULL) return EXIT_FAILURE;
    push_front(&queue->list, data);
    queue->length++;
    return EXIT_SUCCESS;
}

int16_t dequeue(queue_t *queue) {
    if (queue == NULL) return EXIT_FAILURE;
    gdata_t temp = peak_back(&queue->list);
    if (temp != NULL) pop_back(&queue->list);
    queue->length--;
    return EXIT_SUCCESS;
}

size_t queue_length(queue_t *queue) {
    if (queue == NULL) return 0;
    return queue->length;
}

gdata_t queue_front(queue_t *queue) {
    if (queue == NULL) return NULL;
    return peak_back(&queue->list);
}

gdata_t queue_back(queue_t *queue) {
    if (queue == NULL) return NULL;
    return peak_front(&queue->list);
}

bool queue_empty(queue_t *queue) {
    if (queue == NULL || queue->length == 0) return true;
    return false;
}

bool queue_find(queue_t *heystack, gdata_t needle, bool (*search_fun)(lnode_t *node, gdata_t data)) {
    if (queue_empty(heystack)) return false;

    list_itr_t itr = {0};
    list_itr_init(&itr, &heystack->list);
    itr_set_begin(&itr, list_head(&heystack->list));
    lnode_t *temp = itr_begin(&itr);

    bool r = false;
    while (temp != NULL && !r) {
        if (search_fun(temp, needle)) r = true;
        temp = next(&itr);
    }
    return r;
}

void dump_queue(queue_t *queue, void (*_print_data)(gdata_t data)) {
    reverse_dump_list(&queue->list, _print_data);
    printf("\n");
}

void queue_destroy(queue_t *queue) {
    list_destroy(&queue->list);
}
