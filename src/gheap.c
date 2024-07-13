#include "../include/garraylist.h"
#include "../include/gtree.h"
#include <stdio.h>
#include <string.h>

static int aint(anode_t *node) {
    return *(int *)anode_data(node);
}

static void anode_swap_data(anode_t *from, anode_t *to) {
    gdata_t temp = NULL;
    temp = to->data;
    to->data = from->data;
    from->data = temp;
}

static anode_t *node_at(heap_t *heap, ssize_t pos) {
    if (!heap) return NULL;
    if (pos < 0 || (size_t)pos >= alist_size(&heap->buf)) return NULL;
    return &heap->buf.buf[pos];
}

static anode_t *node_child(heap_t *heap, size_t n, ssize_t pos) {
    if (!heap) return NULL;
    return node_at(heap, heap->k * pos + 1 + n);
}

static anode_t *node_parent(heap_t *heap, ssize_t pos) {
    if (!heap) return NULL;
    return node_at(heap, (pos - 1) / heap->k);
}

static ssize_t child_pos(heap_t *heap, size_t n, ssize_t pos) {
    if ((size_t)pos >= alist_size(&heap->buf)) return -1;
    return heap->k * pos + 1 + n;
}

static ssize_t parent_pos(heap_t *heap, ssize_t pos) {
    if (pos <= 0 || (size_t)pos >= alist_size(&heap->buf)) return -1;
    return (pos - 1) / heap->k;
}

static bool check_heap_prop(heap_t *heap, anode_t *parent, anode_t *child) {
    int result = heap->cmp_fun(anode_data(parent), anode_data(child));
    return (heap->type == MAX_HEAP ? result >= 0 : result <= 0);
}

bool valid_heap(heap_t *heap, size_t pos) {
    bool     fin = true;
    anode_t *cur = node_at(heap, pos), *child = NULL;
    ssize_t  child_p = -1;
    if (!cur) return true;
    for (size_t k = 0; k < heap->k; k++) {
        child_p = child_pos(heap, k, pos);
        child = node_at(heap, child_p);
        if (!child) return true;
        fin = fin && check_heap_prop(heap, cur, child) && valid_heap(heap, child_p);
    }
    return fin;
}

void heapify_child(heap_t *heap, size_t child_pos) {
    size_t   p_pos = parent_pos(heap, child_pos);
    anode_t *parent = node_at(heap, p_pos);
    anode_t *child = node_at(heap, child_pos);
    if (!child || !parent || check_heap_prop(heap, parent, child)) return;
    anode_swap_data(child, parent);
    heapify_child(heap, p_pos);
}

void heapify_parent(heap_t *heap, size_t parent_p) {
    anode_t *parent = node_at(heap, parent_p);
    if (!parent) return;
    for (size_t k = 0; k < heap->k; k++) {
        size_t   child_p = child_pos(heap, k, parent_p);
        anode_t *child = node_at(heap, child_p);
        if (!child || check_heap_prop(heap, parent, child)) return;
        anode_swap_data(child, parent);
        heapify_parent(heap, child_p);
    }
}

void build_heap_h(heap_t *heap, size_t pos) {
    anode_t *cur = node_at(heap, pos);
    if (!cur) return;

    for (size_t n = 0; n < heap->k; n++) {
        ssize_t  child_p = child_pos(heap, n, pos);
        anode_t *child = node_at(heap, child_p);
        if (!child) return;
        if (!check_heap_prop(heap, cur, child)) {
            heapify_child(heap, child_p);
        }
        build_heap_h(heap, child_p);
    }
}

// TODO: takes array elements and return valid heap
void build_heap(heap_t *heap) {
    build_heap_h(heap, 0);
}

gdata_t heap_peak(heap_t *heap) {
    return alist_at(&heap->buf, 0);
}

heap_t *heap_create(size_t item_size, size_t k, HEAP_TYPE type) {
    heap_t *heap = malloc(sizeof(heap_t));
    memset(heap, 0, sizeof(heap_t));
    heap_init(heap, item_size, k, type);
    return heap;
}

void heap_init(heap_t *heap, size_t item_size, size_t k, HEAP_TYPE type) {
    alist_init(&heap->buf, item_size);
    heap->k = k;
    heap->type = type;
}

void heap_set_allocator(heap_t *heap, allocator_fun_t allocator_fun) {
    heap->buf.allocator_fun = allocator_fun;
}

void heap_set_cmp_fun(heap_t *heap, cmp_fun cmp) {
    heap->cmp_fun = cmp;
}

// TODO: return error;
void heap_add_safe(heap_t *heap, size_t item_size, gdata_t data) {
    if (!heap->cmp_fun) {
        fprintf(stderr, "Error No Compare Functoin\n");
        return;
    }
    alist_push_safe(&heap->buf, item_size, data);
    heapify_child(heap, alist_size(&heap->buf) - 1);
}

void heap_add(heap_t *heap, const gdata_t data) {
    heap_add_safe(heap, heap->buf.item_size, data);
}

void heap_pop(heap_t *heap) {
    ssize_t  last_pos = alist_size(&heap->buf) - 1;
    anode_t *last = node_at(heap, last_pos);
    anode_swap_data(node_at(heap, 0), last);
    alist_pop(&heap->buf);
    heapify_parent(heap, 0);
}

void heap_destroy(heap_t *heap) {
    alist_destroy(&heap->buf);
}

void heap_for_each(heap_t *heap, for_each_fn function) {
    for (size_t j = 0; j < alist_size(&heap->buf); j++)
        function((anode_t *)&heap->buf.buf[j], 0);
}
