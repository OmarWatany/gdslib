#include "../include/gtree.h"
#include "../include/gqueue.h"
#include "../include/gringbuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define left(node) tnode_child(node, 0)
#define right(node) tnode_child(node, 1)

void heap_add_tnode(heap_t *heap, tnode_t *node);

static void bf_order(ktree_t *tree, for_each_fn for_each) {
    queue_t lvlq = {0};
    queue_init(&lvlq, sizeof(tnode_t *));

    tnode_t *front = tree->root;
    if (front) enqueue(&lvlq, &front);
    while (!queue_empty(&lvlq)) {
        front = *(tnode_t **)queue_front(&lvlq);
        for_each(front, 0);
        for (size_t k = 0; k < tree->k; k++) {
            tnode_t *child = tnode_child(front, k);
            if (child) enqueue(&lvlq, &child);
        }
        dequeue(&lvlq);
    }
    queue_destroy(&lvlq);
}

static void in_order(tnode_t *node, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    in_order(left(node), lvl + 1, for_each);
    for_each(node, lvl);
    in_order(right(node), lvl + 1, for_each);
}

static void post_order(tnode_t *node, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    post_order(left(node), lvl + 1, for_each);
    post_order(right(node), lvl + 1, for_each);
    for_each(node, lvl);
}

static void pre_order(tnode_t *node, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    for_each(node, lvl);
    pre_order(left(node), lvl + 1, for_each);
    pre_order(right(node), lvl + 1, for_each);
}

static void (*order_functions[])(tnode_t *, size_t, for_each_fn) = {
    pre_order,
    in_order,
    post_order,
};

static gdata_t kt_alloc(ktree_t *tree, size_t item_size, gdata_t data) {
    if (tree == NULL || tree == NULL) return NULL;
    return tree->allocator_fun ? tree->allocator_fun(data)
                               : default_safe_allocator(tree->item_size, item_size, data);
}

ktree_t *kt_create(size_t item_size, size_t k) {
    ktree_t *temp = malloc(sizeof(ktree_t));
    memset(temp, 0, sizeof(ktree_t));
    temp->item_size = item_size;
    temp->k = k;
    return temp;
}

void kt_init(ktree_t *tree, size_t item_size, size_t k) {
    if (tree == NULL) return;
    memset(tree, 0, sizeof(ktree_t));
    tree->item_size = item_size;
    tree->k = k;
}

void kt_set_allocator(ktree_t *tree, allocator_fun_t allocator_fun) {
    tree->allocator_fun = allocator_fun;
}

static void kt_node_destroy(tnode_t *node, size_t lvl) {
    (void)lvl;
    tnode_destroy(node);
    free(node);
}

void kt_destroy(ktree_t *tree) {
    kt_for_each(tree, POST_ORDER, kt_node_destroy);
}

void kt_for_each(ktree_t *tree, TRAVERSE_ORDER order, for_each_fn function) {
    if (order == BREADTH_FIRST_ORDER) {
        bf_order(tree, function);
    } else
        order_functions[order](tree->root, 0, function);
}

tnode_t **kt_grand_childrens(ktree_t *tree, size_t lvl) {
    return tnode_grand_children(tree->root, tree->k, lvl);
}

static void tnode_swap_data(tnode_t *from, tnode_t *to) {
    gdata_t temp = NULL;
    temp = to->data;
    to->data = from->data;
    from->data = temp;
}

// heap
bool heapify_safe(heap_t *heap, tnode_t *node, tnode_t *new_node) {
    int restult = heap->in.cmp_fun(tnode_data(new_node), tnode_data(node));

    if ((heap->type == MAX_HEAP && restult > 0) || (heap->type == MIN_HEAP && restult < 0)) {
        tnode_swap_data(new_node, node);
        heap_add_tnode(heap, new_node);
        return true;
    }
    return false;
}

void heapify(heap_t *heap, tnode_t *root) {
    for (size_t k = 0; k < heap->in.k; k++) {
        tnode_t *child = tnode_child(root, k);
        if (!child) return;

        int restult = heap->in.cmp_fun(tnode_data(child), tnode_data(root));
        if ((heap->type == MAX_HEAP && restult > 0) || (heap->type == MIN_HEAP && restult < 0)) {
            tnode_swap_data(root, child);
        }
        for (int i = 0; i < 2; i++)
            heapify(heap, child);
    }
}

size_t heap_tnode_height(tnode_t *root) {
    if (!root) return 0;
    return 1 + heap_tnode_height(tnode_child(root, 0));
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
                tnode_set_link(front, k, node);
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

void heap_add_safe(heap_t *heap, size_t item_size, const gdata_t data) {
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

    for (size_t k = 0; k < heap->in.k; k++)
        if (tnode_child(heap->lastParent, k)) last = tnode_child(heap->lastParent, k);
    tnode_swap_data(heap->in.root, last);

    tnode_set_link(heap->lastParent, dir, 0);
    tnode_destroy(last);
    free(last);
    heapify(heap, heap->in.root);
}

void heap_destroy(heap_t *heap) {
    kt_destroy(&heap->in);
}

void heap_for_each(heap_t *heap, TRAVERSE_ORDER order, for_each_fn function) {
    if (order == BREADTH_FIRST_ORDER) {
        bf_order(&heap->in, function);
    } else
        order_functions[order](heap->in.root, 0, function);
}

// Binary tree

btree_t *bt_create(size_t item_size) {
    ktree_t *tree = kt_create(item_size, 2);
    return tree;
}

void bt_init(ktree_t *tree, size_t item_size, int (*cmp_fun)(gdata_t data1, gdata_t data2)) {
    kt_init(tree, item_size, 2);
    tree->cmp_fun = cmp_fun;
}

// BST

static void bst_add_h(ktree_t *tree, tnode_t *node, gdata_t data) {
    tnode_t *temp = NULL;

    int res = tree->cmp_fun(tnode_data(node), data);
    int dir = -1;
    dir = res > 0 ? 0 : (res < 0 ? 1 : dir);

    if (dir < 0) return;
    temp = tnode_child(node, dir);

    if (!temp) {
        tnode_t *new_node = tnode_create(tree->k);
        tnode_set_data(new_node, kt_alloc(tree, tree->item_size, data));
        tnode_set_link(node, dir, new_node);
        return;
    } else {
        bst_add_h(tree, temp, data);
    }
}

void bst_add(btree_t *tree, gdata_t data) {
    if (!tree->cmp_fun) {
        fprintf(stderr, "Error: there isn't compare function\n");
        return;
    }
    if (!tree->root) {
        tnode_t *new_node = tnode_create(tree->k);
        if (tree->allocator_fun) {
            tnode_set_data(new_node, tree->allocator_fun(data));
        } else
            tnode_set_data(new_node, default_allocator(tree->item_size, data));
        tree->root = new_node;
    } else
        bst_add_h(tree, tree->root, data);
    // tree->size++;
}

static tnode_t *bst_min_max(tnode_t *node, size_t direction) {
    tnode_t *temp = tnode_child(node, direction);
    if (temp) {
        return bst_min_max(temp, direction);
    } else
        return node;
}

static tnode_t *bst_right_min(tnode_t *node) {
    return bst_min_max(tnode_child(node, 1), 0);
}

gdata_t *bst_min(ktree_t *tree) {
    return tnode_data(bst_min_max(tree->root, 0));
}

gdata_t *bst_max(ktree_t *tree) {
    return tnode_data(bst_min_max(tree->root, 1));
}

// check on root outside this function so assume node is root's child
static tnode_t *bst_find_parent(btree_t *heystack, tnode_t *parent, gdata_t needle) {
    if (!parent) return NULL;
    int res = heystack->cmp_fun(tnode_data(parent), needle);
    int dir = -1;
    dir = res > 0 ? 0 : (res < 0 ? 1 : -1);
    if (dir < 0) return parent;

    tnode_t *child = tnode_child(parent, dir);
    if (!child) return NULL;
    res = heystack->cmp_fun(tnode_data(child), needle);
    dir = res > 0 ? 0 : (res < 0 ? 1 : -1);
    return res == 0 ? parent : (dir < 0 ? NULL : bst_find_parent(heystack, child, needle));
}

static tnode_t *bst_find_h(btree_t *heystack, tnode_t *node, gdata_t needle) {
    if (!node) return 0;
    int res = heystack->cmp_fun(tnode_data(node), needle);
    int dir = 0;
    dir = res > 0 ? 0 : (res < 0 ? 1 : -1);
    return res == 0 ? node : bst_find_h(heystack, tnode_child(node, dir), needle);
}

bool bst_find(btree_t *heystack, gdata_t needle) {
    if (!heystack || !heystack->cmp_fun) return 0;
    return bst_find_h(heystack, heystack->root, needle);
}

static void bst_delete_node(tnode_t *parent, tnode_t *node, int dir) {
    tnode_destroy(node);
    free(node);
    tnode_set_link(parent, dir, 0);
}

static void bst_delete_h(ktree_t *tree, tnode_t *node, gdata_t data) {

    tnode_t *parent = bst_find_parent(tree, node, data);
    if (!parent) return;
    tnode_t *child = bst_find_h(tree, parent, data);

    int top = 0, count = 0, dir = 0;
    (void)top;

    for (int i = 0; i < 2; i++)
        if (tnode_child(child, i)) {
            count++;
        }

    dir = child == tnode_child(parent, 0) ? 0 : 1;
    if (count > 1) {
        tnode_t *replacement = bst_right_min(child);
        tnode_t *rep_parent = bst_find_parent(tree, child, replacement->data);
        tnode_swap_data(replacement, child);
        dir = replacement == tnode_child(rep_parent, 0) ? 0 : 1;
        bst_delete_h(tree, rep_parent, data);
    } else if (count == 1) {
        if (child->links[0])
            tnode_set_link(parent, dir, tnode_child(child, 0));
        else
            tnode_set_link(parent, dir, tnode_child(child, 1));
        tnode_destroy(child);

        free(child);
    } else {
        bst_delete_node(parent, child, dir);
    }
}

void bst_delete(btree_t *tree, gdata_t data) {
    bst_delete_h(tree, tree->root, data);
}

void bst_destroy(btree_t *tree) {
    kt_destroy(tree);
}
