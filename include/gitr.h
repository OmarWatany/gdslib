#ifndef ITRERATOR_H_
#define ITRERATOR_H_

#include "gds_types.h"

#define gitr_next(itr)  ((itr)->vtable->next((itr)))
#define gitr_prev(itr)  ((itr)->vtable->prev((itr)))
#define gitr_begin(itr) ((itr)->context->begin)
#define gitr_end(itr)   ((itr)->context->end)
#define gitr_set_from(itr, F)                                                                      \
    do {                                                                                           \
        (itr)->context->from = (F);                                                                \
    } while (0)

gitr_t list_gitr(list_t *list);
void   gitr_destroy(gitr_t *itr);

#endif // !ITRERATOR_H_
