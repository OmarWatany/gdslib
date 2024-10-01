#include "gitr.h"
#include "gds_types.h"
#include "gnode.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// list
typedef struct {
    itr_context_t context;
    list_t       *list;
    uintptr_t     prev_node, next_node;
} list_itr_context_t;

gdata_t list_gitr_next(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_context_t      *context = itr->context;
    list_itr_context_t *lcontext = (list_itr_context_t *)context;

    lnode_t *from = (lnode_t *)context->from;
    if (from == NULL) {
        if (lcontext->list->head) {
            from = lcontext->list->head;
        } else
            return NULL;
    }
    if (lnode_link(from) == 0) {
        return NULL;
    } else if (from == lcontext->list->head) {
        lcontext->next_node = lnode_link(lcontext->list->head);
    } else {
        lcontext->next_node = lcontext->prev_node ^ lnode_link(from);
    }
    lcontext->prev_node = (uintptr_t)from;
    context->from = (gnode_t *)lcontext->next_node;
    lcontext->next_node = lcontext->prev_node ^ lnode_link(from);
    return (gnode_t *)context->from;
}

gdata_t list_gitr_prev(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_context_t      *context = itr->context;
    list_itr_context_t *lcontext = (list_itr_context_t *)context;
    lnode_t            *from = (lnode_t *)context->from;
    if (from == NULL) {
        if (lcontext->list->tail) {
            from = lcontext->list->tail;
        } else
            return NULL;
    }
    if (lnode_link(from) == 0) {
        return NULL;
    } else if (from == lcontext->list->tail) {
        lcontext->prev_node = lnode_link(lcontext->list->tail);
    } else {
        lcontext->prev_node = lcontext->next_node ^ lnode_link(from);
    }
    lcontext->next_node = (uintptr_t)from;
    context->from = (gnode_t *)lcontext->prev_node;
    lcontext->prev_node = lcontext->next_node ^ lnode_link(from);
    return (gnode_t *)context->from;
}

gitr_vtable list_itr_vtable = {
    .next = list_gitr_next,
    .prev = list_gitr_prev,
};

gitr_t list_gitr(list_t *list) {
    list_itr_context_t *ctx = calloc(1, sizeof(*ctx));
    *ctx = (list_itr_context_t){
        .list = list,
        .context.from = (gnode_t *)list->head,
        .context.begin = (gnode_t *)list->head,
        .context.end = (gnode_t *)list->tail,
    };
    return (gitr_t){.context = (itr_context_t *)ctx, .vtable = &list_itr_vtable};
}

// tree
// queue
// arraylist
// heap
// stack
// astack
// pqueue
// circular_buffer

// general
void gitr_destroy(gitr_t *itr) {
    if (NULL == itr) return;
    free(itr->context);
}
