#include "../include/gpriority_queue.h"
#include "../include/gtree.h"
#include <stdio.h>
#include <string.h>

void pq_add_node(pqueue_t *heap, tnode_t *node);

static int cmp_pq_node(gdata_t data1, gdata_t data2) {
    if (!(data1 && data2)) return 0;
    long i = ((pq_node *)data1)->priority, j = ((pq_node *)data2)->priority;
    return i > j ? 1 : i < j ? -1 : 0;
}

pqueue_t *pq_create(size_t item_size) {
    pqueue_t *queue = malloc(sizeof(pqueue_t));
    memset(queue, 0, sizeof(pqueue_t));
    pq_init(queue, item_size);
    return queue;
}

void pq_init(pqueue_t *pqueue, size_t item_size) {
    heap_init(&pqueue->h, item_size, 2, MAX_HEAP);
    heap_set_cmp_fun(&pqueue->h, cmp_pq_node);
}

void pq_set_allocator(pqueue_t *pqueue, allocator_fun_t allocator) {
    pqueue->h.in.allocator_fun = allocator;
}

gdata_t pq_peak(pqueue_t *pqueue) {
    return ((pq_node *)heap_peak(&pqueue->h))->data;
}

static gdata_t kt_alloc(ktree_t *tree, size_t item_size, gdata_t data) {
    if (tree == NULL || tree == NULL) return NULL;
    return tree->allocator_fun ? tree->allocator_fun(data)
                               : default_safe_allocator(tree->item_size, item_size, data);
}

static pq_node *pq_node_create(long int priority, gdata_t data) {
    pq_node *temp = (pq_node *)malloc(sizeof(pq_node));
    temp->priority = priority;
    temp->data = data;
    return temp;
}

// TODO: return error
int16_t pq_enqueue_safe(pqueue_t *pqueue, size_t item_size, long int priority, gdata_t data) {
    if (!pqueue->h.in.cmp_fun) {
        fprintf(stderr, "ERROR : NO Compare Function \n");
        return EXIT_FAILURE;
    }
    pq_node *new_pq_node = pq_node_create(priority, kt_alloc(&pqueue->h.in, item_size, data));
    // tnode saves pq address
    tnode_t *new_node = tnode_create(pqueue->h.in.k);
    tnode_set_data(new_node, new_pq_node);
    heap_add_tnode(&pqueue->h, new_node);
    return EXIT_SUCCESS;
}

int16_t pq_enqueue(pqueue_t *pqueue, long int priority, gdata_t data) {
    return pq_enqueue_safe(pqueue, pqueue->h.in.item_size, priority, data);
}

int16_t pq_dequeue(pqueue_t *pqueue) {
    pq_node *temp = (pq_node *)heap_peak(&pqueue->h);
    free(temp->data);
    heap_pop(&pqueue->h);
    return EXIT_SUCCESS;
}

static void kt_node_destroy(tnode_t *node, size_t lvl) {
    (void)lvl;
    pq_node *temp = (pq_node *)tnode_data(node);
    free(temp->data);
    tnode_destroy(node);
    free(node);
}

void pq_for_each(pqueue_t *pqueue, TRAVERSE_ORDER order, for_each_fn function) {
    kt_for_each(&pqueue->h.in, order, function);
}

void pq_destroy(pqueue_t *pqueue) {
    kt_for_each(&pqueue->h.in, POST_ORDER, kt_node_destroy);
}
