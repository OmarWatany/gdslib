#include "garraylist.h"
#include "gtree.h"

#include <stdio.h>
#include <string.h>

#define PARENT_POS(pos) (((pos) - 1) >> 1)

void heapify_child(heap_t *heap, size_t child_pos);
void heapify_parent(heap_t *heap, size_t parent_p);

static void anode_swap_data(anode_t *from, anode_t *to, size_t size) {
    anode_t temp[size + 1];
    memcpy(temp, to, size);   /* temp = to; */
    memcpy(to, from, size);   /* to = from; */
    memcpy(from, temp, size); /* from = temp; */
}

static anode_t *node_at(heap_t *heap, ssize_t pos) {
    if (!heap) return NULL;
    if (pos < 0 || (size_t)pos >= alist_size(&heap->buf)) return NULL;
    return alist_at(&heap->buf, pos);
}

static ssize_t child_pos(heap_t *heap, size_t n, ssize_t pos) {
    if ((size_t)pos >= alist_size(&heap->buf)) return -1;
    return (pos << 1) + 1 + n;
}

static bool check_heap_prop(heap_t *heap, anode_t *parent, anode_t *child) {
    int result = heap->cmp_fun(parent, child);
    return (heap->type == MAX_HEAP ? result >= 0 : result <= 0);
}

bool valid_heap(heap_t *heap, size_t pos) {
    bool     fres = true;
    anode_t *cur = node_at(heap, pos), *child = NULL;
    ssize_t  child_p = -1;
    if (!cur) return true;
    for (size_t k = 0; k < 2; k++) {
        child_p = child_pos(heap, k, pos);
        child = node_at(heap, child_p);
        if (!child) return true;
        fres = fres && check_heap_prop(heap, cur, child) && valid_heap(heap, child_p);
    }
    return fres;
}

void heapify_child(heap_t *heap, size_t child_pos) {
    size_t   parent_p = PARENT_POS(child_pos);
    anode_t *parent = node_at(heap, parent_p);
    anode_t *child = node_at(heap, child_pos);
    if (!child || !parent || check_heap_prop(heap, parent, child)) return;
    anode_swap_data(child, parent, heap->buf.item_size);
    heapify_child(heap, parent_p);
}

void heapify_parent(heap_t *heap, size_t parent_p) {
    anode_t *parent = node_at(heap, parent_p);
    if (!parent) return;
    size_t n = 0;
    size_t largest_pos = parent_p;
    size_t child_p = child_pos(heap, n, largest_pos);
    if (!node_at(heap, child_p)) return;
    for (size_t n = 1; n <= 2; n++) {
        anode_t *child = node_at(heap, child_p);
        if (!child) continue;
        if (!check_heap_prop(heap, node_at(heap, largest_pos), child)) largest_pos = child_p;
        child_p = child_pos(heap, n, largest_pos);
    }
    if (largest_pos == parent_p) return;
    anode_swap_data(node_at(heap, largest_pos), parent, heap->buf.item_size);
    heapify_parent(heap, largest_pos);
}

static void for_each_h(anode_t *node, size_t lvl, for_each_fn for_each_f) {
    // iprint(&(tree_for_data){node, lvl})
    for_each_f(&(tree_for_data){node, lvl});
}

static void in_order(heap_t *heap, size_t pos, size_t lvl, for_each_fn for_each) {
    if (!heap || pos >= heap->buf.size) return;
    size_t child_p = 0;
    for (size_t n = 0; n < 2; n++) {
        child_p = child_pos(heap, n, pos);
        in_order(heap, child_p, lvl + 1, for_each);
        if (n == (2 - 1) / 2) for_each_h(node_at(heap, pos), lvl, for_each);
    }
}

static void post_order(heap_t *heap, size_t pos, size_t lvl, for_each_fn for_each) {
    if (!heap) return;
    for (size_t j = 0; j < 2; j++)
        post_order(heap, child_pos(heap, j, pos), lvl + 1, for_each);
    for_each_h(node_at(heap, pos), lvl, for_each);
}

static void pre_order(heap_t *heap, size_t pos, size_t lvl, for_each_fn for_each) {
    if (!heap) return;
    for_each_h(node_at(heap, pos), lvl, for_each);
    for (size_t j = 0; j < 2; j++)
        pre_order(heap, child_pos(heap, j, pos), lvl + 1, for_each);
}

static void (*order_functions[])(heap_t *heap, size_t k, size_t lvl, for_each_fn) = {
    [PRE_ORDER] = pre_order,
    [IN_ORDER] = in_order,
    [POST_ORDER] = post_order,
};

gdata_t heap_peak(heap_t *heap) {
    return alist_at(&heap->buf, 0);
}

heap_t *heap_create(size_t item_size, HEAP_TYPE type) {
    heap_t *heap = malloc(sizeof(heap_t));
    memset(heap, 0, sizeof(heap_t));
    heap_init(heap, item_size, type);
    return heap;
}

void heap_init(heap_t *heap, size_t item_size, HEAP_TYPE type) {
    alist_init(&heap->buf, item_size);
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
        fprintf(stderr, "Error No Compare Function\n");
        return;
    }
    size_t old_size = heap->buf.size;
    alist_push_safe(&heap->buf, item_size, data);
    if (heap->buf.size == old_size) heap->buf.size++;
    heapify_child(heap, alist_size(&heap->buf) - 1);
}

void heap_add(heap_t *heap, const gdata_t data) {
    heap_add_safe(heap, heap->buf.item_size, data);
}

void heap_pop(heap_t *heap) {
    ssize_t  last_pos = heap->buf.size - 1;
    anode_t *last = node_at(heap, last_pos);
    anode_swap_data(node_at(heap, 0), last, heap->buf.item_size);
    heap->buf.size--;
    heapify_parent(heap, 0);
}

void heap_destroy(heap_t *heap) {
    alist_destroy(&heap->buf);
}

void heap_for_each_order(heap_t *heap, TRAVERSE_ORDER order, for_each_fn function) {
    switch (order) {
    case BREADTH_FIRST_ORDER:
        for (size_t j = 0; j < heap->buf.size; j++)
            for_each_h(node_at(heap, j), 0, function);
        break;
    default:
        order_functions[order](heap, 0, 0, function);
    }
}

void heap_for_each(heap_t *heap, for_each_fn function) {
    // iprint
    heap_for_each_order(heap, BREADTH_FIRST_ORDER, function);
}
