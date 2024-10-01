#include "gitr.h"
#include "gds_types.h"
#include "gnode.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// list
typedef struct {
    itr_ctx_t context;
    list_t   *list;
    uintptr_t prev_node, next_node;
} list_itr_ctx_t;

gdata_t list_gitr_next(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_ctx_t      *context = itr->context;
    list_itr_ctx_t *lcontext = (list_itr_ctx_t *)context;

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
    itr_ctx_t      *context = itr->context;
    list_itr_ctx_t *lcontext = (list_itr_ctx_t *)context;
    lnode_t        *from = (lnode_t *)context->from;
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
    if (!list) return (gitr_t){0};
    list_itr_ctx_t *ctx = calloc(1, sizeof(*ctx));
    *ctx = (list_itr_ctx_t){
        .list = list,
        .context.from = (gnode_t *)list->head,
        .context.begin = (gnode_t *)list->head,
        .context.end = (gnode_t *)list->tail,
    };
    return (gitr_t){.context = (itr_ctx_t *)ctx, .vtable = &list_itr_vtable};
}

// arraylist
gdata_t alist_gitr_next(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_ctx_t *ctx = itr->context;
    if (ctx->from == ctx->end) return NULL;
    return (ctx->from += sizeof(anode_t));
}

gdata_t alist_gitr_prev(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_ctx_t *ctx = itr->context;
    if (ctx->from == ctx->begin) return NULL;
    return (ctx->from -= sizeof(anode_t));
}

gitr_vtable alist_itr_vtable = {
    .next = alist_gitr_next,
    .prev = alist_gitr_prev,
};

gitr_t alist_gitr(alist_t *lst) {
    if (!lst) return (gitr_t){0};
    itr_ctx_t *ctx = calloc(1, sizeof(*ctx));
    *ctx = (itr_ctx_t){
        .from = &lst->buf[0],
        .begin = &lst->buf[0],
        .end = &lst->buf[lst->size - 1],
    };
    return (gitr_t){.context = (itr_ctx_t *)ctx, .vtable = &alist_itr_vtable};
}

// stack
gitr_t stack_gitr(stack_t *stack) {
    return list_gitr(&stack->list);
}

// queue
gitr_t queue_gitr(queue_t *queue) {
    return list_gitr(&queue->list);
}

// astack
gitr_t astack_gitr(astack_t *stack) {
    return alist_gitr(&stack->buf);
}
//
gitr_t carray_gitr(circular_array_t *carr) {
    if (!carr) return (gitr_t){0};
    itr_ctx_t *ctx = calloc(1, sizeof(*ctx));
    *ctx = (itr_ctx_t){
        .from = &carr->buf[0],
        .begin = &carr->buf[0],
        .end = &carr->buf[carr->size - 1],
    };
    return (gitr_t){.context = (itr_ctx_t *)ctx, .vtable = &alist_itr_vtable};
}

// tree
// heap
// pqueue
// circular_buffer

// general
void gitr_destroy(gitr_t *itr) {
    if (NULL == itr) return;
    if (itr->context) free(itr->context);
}
