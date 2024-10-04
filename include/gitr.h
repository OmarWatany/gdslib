#ifndef ITRERATOR_H_
#define ITRERATOR_H_

#include "gds_types.h"

// return NODE
#define itr_next(itr)  ((!(itr)->vtable) ? NULL : (itr)->vtable->next((itr)))
#define itr_prev(itr)  ((!(itr)->vtable) ? NULL : (itr)->vtable->prev((itr)))
#define itr_begin(itr) ((!(itr)->context) ? NULL : (itr)->context->begin)
#define itr_end(itr)   ((!(itr)->context) ? NULL : (itr)->context->end)

#define gitr_set_from(itr, F)                                                                      \
    do {                                                                                           \
        (itr)->context->from = (F);                                                                \
    } while (0)

gitr_t list_gitr(list_t *list);
gitr_t stack_gitr(stack_t *stack);
gitr_t queue_gitr(queue_t *queue);
gitr_t alist_gitr(alist_t *alist);
gitr_t astack_gitr(astack_t *astack);
gitr_t tr_gitr_o(ktree_t *tr, TRAVERSE_ORDER order);

#define heap_gitr(H)    (alist_gitr(&(H)->buf))
#define pqueue_gitr(PQ) (heap_gitr(&(PQ)->h))
#define tr_gitr(T)      (tr_gitr_o((T), IN_ORDER))

void gitr_destroy(gitr_t *itr);

#endif // !ITRERATOR_H_
