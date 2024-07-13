#include "../include/gpriority_queue.h"
#include "../include/gtree.h"
#include <stdio.h>
#include <string.h>

void pq_add_node(pqueue_t *heap, anode_t *node);

static int cmp_pq_node(gdata_t data1, gdata_t data2) {
    if (!(data1 && data2)) return 0;
    int i = ((pq_node *)data1)->priority, j = ((pq_node *)data2)->priority;
    return i > j ? 1 : i < j ? -1 : 0;
}

pqueue_t *pq_create(size_t item_size, HEAP_TYPE type) {
    pqueue_t *queue = malloc(sizeof(pqueue_t));
    memset(queue, 0, sizeof(pqueue_t));
    pq_init(queue, item_size, type);
    return queue;
}

void pq_init(pqueue_t *pqueue, size_t item_size, HEAP_TYPE type) {
    heap_init(&pqueue->h, sizeof(pq_node), 2, type);
    heap_set_cmp_fun(&pqueue->h, cmp_pq_node);
    pqueue->item_size = item_size;
}

void pq_set_allocator(pqueue_t *pqueue, allocator_fun_t allocator) {
    pqueue->h.buf.allocator_fun = allocator;
}

pq_node *pq_peak_node(pqueue_t *pqueue) {
    return ((pq_node *)heap_peak(&pqueue->h));
}
gdata_t pq_peak(pqueue_t *pqueue) {
    return ((pq_node *)pq_peak_node(pqueue))->data;
}

static gdata_t pq_alloc(pqueue_t *q, size_t item_size, gdata_t data) {
    if (!q) return NULL;
    return q->h.buf.allocator_fun ? q->h.buf.allocator_fun(data)
                                  : default_safe_allocator(q->item_size, item_size, data);
}

static pq_node pq_node_create(long int priority, gdata_t data) {
    pq_node temp = {0};
    temp.priority = priority;
    temp.data = data;
    return temp;
}

// TODO: return error
int16_t pq_enqueue_safe(pqueue_t *pqueue, size_t item_size, long int priority, gdata_t data) {
    if (!pqueue->h.cmp_fun) {
        fprintf(stderr, "ERROR : No Compare Function \n");
        return EXIT_FAILURE;
    }
    pq_node new_pq_node = pq_node_create(priority, pq_alloc(pqueue, item_size, data));
    heap_add_safe(&pqueue->h, sizeof(pq_node), &new_pq_node);
    return EXIT_SUCCESS;
}

int16_t pq_enqueue(pqueue_t *pqueue, long int priority, gdata_t data) {
    return pq_enqueue_safe(pqueue, pqueue->item_size, priority, data);
}

int16_t pq_dequeue(pqueue_t *pqueue) {
    free(pq_peak_node(pqueue)->data);
    heap_pop(&pqueue->h);
    return EXIT_SUCCESS;
}

static void heap_node_destroy(gdata_t node, size_t lvl) {
    (void)lvl;
    pq_node *temp = (pq_node *)anode_data((anode_t *)node);
    free(temp->data);
}

void pq_for_each(pqueue_t *pqueue, for_each_fn function) {
    heap_for_each(&pqueue->h, function);
}

void pq_destroy(pqueue_t *pqueue) {
    heap_for_each(&pqueue->h, heap_node_destroy);
    heap_destroy(&pqueue->h);
}
