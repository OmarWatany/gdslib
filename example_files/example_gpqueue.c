#include "../include/gpriority_queue.h"
#include <stdio.h>

// TODO : pq_node for_each system
static void iprint(gdata_t node, size_t lvl) {
    (void)lvl;
    if (!node) return;
    int  value = *(int *)((pq_node *)tnode_data(node))->data;
    long priority = ((pq_node *)tnode_data(node))->priority;
    printf("value %d - priority %ld\n", value, priority);
}

static inline void pq_enqueue_int(pqueue_t *pqueue, long priority, int i) {
    pq_enqueue(pqueue, priority, &i);
}

static inline int pq_peak_int(pqueue_t *pqueue) {
    return *(int *)pq_peak(pqueue);
}

void pq_dump(pqueue_t *pq) {
    printf("BREADTH FIRST PRINT\n");
    pq_for_each(pq, iprint);
    printf("-----------------------\n");
}

void test_deq(pqueue_t *pq) {
    pq_enqueue_int(pq, 9, 0x45);
    pq_dump(pq);
    printf("peak %d\n", pq_peak_int(pq));
    printf("DEQUEUEING...\n");
    pq_dequeue(pq);
    pq_dump(pq);
}

void test_enq(pqueue_t *pq) {
    pq_enqueue_int(pq, 3, 23);
    pq_enqueue_int(pq, 4, 29);
    pq_enqueue_int(pq, 5, 9);
    pq_enqueue_int(pq, -99999, 9);

    printf("peak %d\n", pq_peak_int(pq));

    pq_dump(pq);
}

int main() {
    pqueue_t qu = {0};
    pq_init(&qu, sizeof(int), MAX_HEAP);
    test_enq(&qu);
    test_deq(&qu);
    pq_destroy(&qu);
    return 0;
}
