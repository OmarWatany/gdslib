#include "gqueue.h"
#include "gitr.h"
#include "glinkedlist.h"
#include <stdio.h>
#include <stdlib.h>

queue_t *queue_create(size_t item_size) {
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    if (queue == NULL) return NULL;
    queue_init(queue, item_size);
    return queue;
}

void queue_init(queue_t *queue, size_t item_size) {
    list_init(&queue->list, item_size);
    queue->length = 0;
}

int16_t enqueue_safe(queue_t *queue, size_t item_size, gdata_t data) {
    if (queue == NULL) return EXIT_FAILURE;
    push_front_safe(&queue->list, item_size, data);
    queue->length++;
    return EXIT_SUCCESS;
}

int16_t enqueue(queue_t *queue, gdata_t data) {
    return enqueue_safe(queue, queue->list.item_size, data);
}

int16_t dequeue(queue_t *queue) {
    if (queue == NULL) return EXIT_FAILURE;
    int16_t rc = pop_back(&queue->list, NULL);
    if (!rc) queue->length--;
    return rc;
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

bool queue_find(queue_t *heystack, gdata_t needle, bool (*search_fun)(gdata_t d1, gdata_t d2)) {
    if (queue_empty(heystack)) return false;

    gitr_t   itr = list_gitr(&heystack->list);
    lnode_t *temp = itr_begin(&itr);

    bool r = false;
    while (temp != NULL && !r) {
        if (!search_fun(temp, needle)) r = true;
        temp = itr_next(&itr);
    }
    gitr_destroy(&itr);
    return r;
}

void queue_dump(queue_t *queue, void (*_print_data)(gdata_t data)) {
    reverse_dump_list(&queue->list, _print_data);
    printf("\n");
}

void queue_destroy(queue_t *queue) {
    list_destroy(&queue->list);
}
