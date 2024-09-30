#ifndef ITRERATOR_H_
#define ITRERATOR_H_

#include "gds_types.h"

#define gitr_next(itr)  ((itr)->vtable->next((itr)))
#define gitr_prev(itr)  ((itr)->vtable->prev((itr)))
#define gitr_begin(itr) ((itr)->context->begin)
#define gitr_end(itr)   ((itr)->context->end)

/*
gnode_t *gitr_next(gitr_t *itr, itr_context_t *context);
gnode_t *gitr_prev(gitr_t *itr, itr_context_t *context);
*/

gitr_t *list_gitr_create(list_t *list);
void    gitr_destroy(gitr_t *itr);

#endif // !ITRERATOR_H_
