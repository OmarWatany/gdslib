#include "../include/gringbuffer.h"
#include "../include/gtree.h"
#include <stdio.h>
#include <string.h>

static void tnode_swap_data(tnode_t *from, tnode_t *to) {
    gdata_t temp = NULL;
    temp = to->data;
    to->data = from->data;
    from->data = temp;
}

static gdata_t kt_alloc(ktree_t *tree, size_t item_size, gdata_t data) {
    if (tree == NULL || tree == NULL) return NULL;
    return tree->allocator_fun ? tree->allocator_fun(data)
                               : default_safe_allocator(tree->item_size, item_size, data);
}

// heap

void heapify(heap_t *heap, tnode_t *root) {
    for (size_t k = 0; k < heap->in.k; k++) {
        tnode_t *child = tnode_child(root, k);
        if (!child) return;

        int root_res = heap->in.cmp_fun(tnode_data(child), tnode_data(heap->in.root));
        if ((heap->type == MAX_HEAP && root_res > 0) || (heap->type == MIN_HEAP && root_res < 0))
            tnode_swap_data(heap->in.root, child);
        int restult = heap->in.cmp_fun(tnode_data(child), tnode_data(root));
        if ((heap->type == MAX_HEAP && restult > 0) || (heap->type == MIN_HEAP && restult < 0))
            tnode_swap_data(root, child);
        for (int i = 0; i < 2; i++)
            heapify(heap, child);
    }
}

size_t heap_tnode_height(tnode_t *root) {
    if (!root) return 0;
    return 1 + heap_tnode_height(tnode_child(root, 0));
}

gdata_t heap_peak(heap_t *heap) {
    return tnode_data(heap->in.root);
}

size_t heap_height(heap_t *heap) {
    return heap_tnode_height(heap->in.root);
}

// breadth insert
// TODO: return error
void heap_add_tnode(heap_t *heap, tnode_t *node) {
    size_t       lvlCount = 1 << heap_height(heap);
    ringbuffer_t Q = {0};
    ring_init(&Q, sizeof(tnode_t *), lvlCount + 2);

    // base case
    tnode_t *front = heap->in.root;
    if (!front) {
        heap->in.root = node;
        goto END_FUNC;
    } else
        ring_write(&Q, &front);

    while (!ring_empty(&Q)) {
        front = *(tnode_t **)ring_read(&Q);
        for (size_t k = 0; k < heap->in.k; k++) {
            tnode_t *child = tnode_child(front, k);
            if (!child) {
                tnode_set_child(front, k, node);
                // tnode_set_link(front, k, node);
                heap->lastParent = front;
                goto END_FUNC;
            }
            ring_write(&Q, &child);
        }
    }

END_FUNC:
    heapify(heap, heap->in.root);
    ring_destroy(&Q);
}

void get_level_nodes(tnode_t *parent, size_t k, size_t lvl, ringbuffer_t *ring) {
    if (!ring) return;

    if (2 == lvl) {

        return;
    }

    get_level_nodes(tnode_child(parent, 0), k, lvl - 1, ring);
}

heap_t *heap_create(size_t item_size, size_t k, HEAP_TYPE type) {
    heap_t *heap = malloc(sizeof(heap_t));
    heap_init(heap, item_size, k, type);
    return heap;
}

void heap_init(heap_t *heap, size_t item_size, size_t k, HEAP_TYPE type) {
    kt_init(&heap->in, item_size, k);
    heap->type = type;
    heap->lastParent = 0;
}

void heap_set_allocator(heap_t *heap, allocator_fun_t allocator_fun) {
    heap->in.allocator_fun = allocator_fun;
}

void heap_set_cmp_fun(heap_t *heap, cmp_fun cmp) {
    heap->in.cmp_fun = cmp;
}

void heap_add_safe(heap_t *heap, size_t item_size, gdata_t data) {
    if (!heap->in.cmp_fun) {
        // TODO: return error;
        fprintf(stderr, "Error No Compare Functoin\n");
        return;
    }
    tnode_t *new_node = tnode_create(heap->in.k);
    tnode_set_data(new_node, kt_alloc(&heap->in, item_size, data));
    heap_add_tnode(heap, new_node);
}

void heap_add(heap_t *heap, const gdata_t data) {
    heap_add_safe(heap, heap->in.item_size, data);
}

gdata_t heap_peek(heap_t *heap) {
    return tnode_data(heap->in.root);
}

void heap_add_sub_heap(heap_t *heap, tnode_t *root) {
    if (!root || !heap) return;
    for (size_t k = 0; k < heap->in.k; k++)
        heap_add_sub_heap(heap, tnode_child(root, k));
    memset(root->links, 0, sizeof(tnode_t *) * heap->in.k);
    heap_add_tnode(heap, root);
}

void heap_pop(heap_t *heap) {
    tnode_t *last = NULL;
    size_t   dir = 0;

    for (size_t k = 0; k < heap->in.k; k++) {
        if (tnode_child(heap->lastParent, k)) {
            last = tnode_child(heap->lastParent, k);
            dir = k;
        }
    }

    tnode_swap_data(heap->in.root, last);
    heap->lastParent->links[dir] = 0;
    tnode_set_child(heap->lastParent, dir, 0);
    tnode_destroy(last);
    free(last);
    heapify(heap, heap->in.root);
}

void heap_destroy(heap_t *heap) {
    kt_destroy(&heap->in);
}

void heap_for_each(heap_t *heap, TRAVERSE_ORDER order, for_each_fn function) {
    kt_for_each(&heap->in, order, function);
}
