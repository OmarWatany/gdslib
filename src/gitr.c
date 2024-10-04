#include "gitr.h"
#include "gds_types.h"
#include "gnode.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define IDATA(N) (((gnode_t *)(N))->data)

// list
typedef struct {
    itr_ctx_t context;
    list_t   *list;
    uintptr_t prev_node, next_node;
} list_itr_ctx_t;

typedef struct {
    itr_ctx_t context;
    size_t    idx;
    ktree_t  *tr;
    tnode_t  *buffer[];
} tr_itr_ctx_t;

gdata_t list_next(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_ctx_t      *ctx = itr->context;
    list_itr_ctx_t *lctx = (list_itr_ctx_t *)ctx;
    lnode_t        *from = (lnode_t *)ctx->from;

    if (!from) return NULL;
    if (lnode_link(from) == 0) {
        return NULL;
    } else if (from == lctx->list->head) {
        lctx->next_node = lnode_link(lctx->list->head);
    } else {
        lctx->next_node = lctx->prev_node ^ lnode_link(from);
    }
    lctx->prev_node = (uintptr_t)from;
    ctx->from = (gnode_t *)lctx->next_node;
    lctx->next_node = lctx->prev_node ^ lnode_link(from);
    return ctx->from ? IDATA(ctx->from) : NULL;
}

gdata_t list_prev(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_ctx_t      *ctx = itr->context;
    list_itr_ctx_t *lctx = (list_itr_ctx_t *)ctx;
    lnode_t        *from = (lnode_t *)ctx->from;

    if (from == NULL) {
        if (lctx->list->tail) {
            from = lctx->list->tail;
            lctx->prev_node = lctx->next_node = 0;
        } else
            return NULL;
    }

    if (lnode_link(from) == 0) {
        return from->data;
    } else if (from == lctx->list->tail) {
        lctx->prev_node = lnode_link(lctx->list->tail);
    } else {
        lctx->prev_node = lctx->next_node ^ lnode_link(from);
    }

    lctx->next_node = (uintptr_t)from;
    ctx->from = (gnode_t *)lctx->prev_node;
    lctx->prev_node = lctx->next_node ^ lnode_link(from);
    return lctx->next_node != (uintptr_t)NULL ? IDATA(lctx->next_node) : NULL;
}

gitr_vtable list_itr_vtable = {
    .next = list_next,
    .prev = list_prev,
};

gitr_t list_gitr(list_t *list) {
    if (!list) return (gitr_t){0};
    list_itr_ctx_t *ctx = calloc(1, sizeof(*ctx));
    *ctx = (list_itr_ctx_t){
        .list = list,
        .context.from = (gnode_t *)list->head,
        .context.begin = (gnode_t *)list->head,
    };
    return (gitr_t){.context = (itr_ctx_t *)ctx, .vtable = &list_itr_vtable};
}

// arraylist
gdata_t alist_next(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_ctx_t *ctx = itr->context;
    if (ctx->from == (ctx->end + sizeof(anode_t))) return NULL;
    return IDATA((ctx->from += sizeof(anode_t)));
}

gdata_t alist_prev(gitr_t *itr) {
    if (NULL == itr) return NULL;
    itr_ctx_t *ctx = itr->context;
    if (ctx->from == ctx->begin) return NULL;
    return IDATA(ctx->from -= sizeof(anode_t));
}

gitr_vtable alist_itr_vtable = {
    .next = alist_next,
    .prev = alist_prev,
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

// circular_buffer
// TEST : test it
gitr_t carray_gitr(circular_array_t *carr) {
    if (!carr) return (gitr_t){0};
    itr_ctx_t *ctx = calloc(1, sizeof(*ctx));
    *ctx = (itr_ctx_t){
        .from = &carr->buf[0],
        .begin = &carr->buf[0],
    };
    return (gitr_t){.context = (itr_ctx_t *)ctx, .vtable = &alist_itr_vtable};
}

// heap
// pqueue

// Tree
void bf_order_h(ktree_t *tree, tnode_t *buffer[]) {
    size_t   write_idx = 0, read_idx = 0;
    tnode_t *front = tree->root;
    buffer[write_idx++] = front;
    while (write_idx < tree->size) {
        front = buffer[read_idx++];
        for (size_t k = 0; k < tree->k; k++) {
            tnode_t *child = tnode_child(front, k);
            if (child) buffer[write_idx++] = child;
        }
    }
}

void in_order_r(tnode_t *node, tnode_t *buffer[], size_t k, size_t *write_idx) {
    if (node == NULL) return;
    for (size_t n = 0; n < k; n++) {
        in_order_r(tnode_child(node, n), buffer, k, write_idx);
        if (n == (k - 1) / 2) buffer[(*write_idx)++] = node;
    }
}

void in_order_h(ktree_t *tree, tnode_t *buffer[]) {
    size_t write_idx = 0;
    in_order_r(tree->root, buffer, tree->k, &write_idx);
}

void pre_order_r(tnode_t *node, tnode_t *buffer[], size_t k, size_t *write_idx) {
    if (node == NULL) return;
    buffer[(*write_idx)++] = node;
    for (size_t n = 0; n < k; n++)
        pre_order_r(tnode_child(node, n), buffer, k, write_idx);
}

void pre_order_h(ktree_t *tree, tnode_t *buffer[]) {
    static size_t write_idx = 0;
    pre_order_r(tree->root, buffer, tree->k, &write_idx);
}

void post_order_r(tnode_t *node, tnode_t *buffer[], size_t k, size_t *write_idx) {
    if (node == NULL) return;
    for (size_t n = 0; n < k; n++)
        post_order_r(tnode_child(node, n), buffer, k, write_idx);
    buffer[(*write_idx)++] = node;
}

void post_order_h(ktree_t *tree, tnode_t *buffer[]) {
    size_t write_idx = 0;
    post_order_r(tree->root, buffer, tree->k, &write_idx);
}

static gdata_t tr_next(gitr_t *itr) {
    itr_ctx_t    *ctx = itr->context;
    tr_itr_ctx_t *tctx = (tr_itr_ctx_t *)itr->context;
    if (tctx->idx == tctx->tr->size - 1) return NULL;
    return IDATA(ctx->from = tctx->buffer[++tctx->idx]);
}

static gdata_t tr_prev(gitr_t *itr) {
    itr_ctx_t    *ctx = itr->context;
    tr_itr_ctx_t *tctx = (tr_itr_ctx_t *)itr->context;
    if (tctx->idx == 0) return tctx->buffer[0];
    return IDATA(ctx->from = tctx->buffer[--tctx->idx]);
}

static gitr_vtable tr_itr_vtable = {
    .next = tr_next,
    .prev = tr_prev,
};

static void (*tr_helpers[])(ktree_t *, tnode_t *[]) = {
    [BREADTH_FIRST_ORDER] = bf_order_h,
    [PRE_ORDER] = pre_order_h,
    [IN_ORDER] = in_order_h,
    [POST_ORDER] = post_order_h,
};

gitr_t tr_gitr_o(ktree_t *tr, TRAVERSE_ORDER order) {
    if (!tr || tr->size == 0) return (gitr_t){0};
    tr_itr_ctx_t *ctx = calloc(1, sizeof(*ctx) + tr->size * sizeof(tnode_t *));
    *ctx = (tr_itr_ctx_t){
        .tr = tr,
        .idx = 0,
    };
    // TODO: add reverse order functions
    tr_helpers[order](tr, ctx->buffer);
    ctx->context.begin = ctx->buffer[0];

    // Select the appropriate vtable based on the order
    return (gitr_t){
        .context = (itr_ctx_t *)ctx,
        .vtable = &tr_itr_vtable,
    };
}

// general
void gitr_destroy(gitr_t *itr) {
    if (NULL == itr) return;
    if (itr->context) free(itr->context);
}
