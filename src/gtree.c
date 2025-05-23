#include "gtree.h"
#include "gnode.h"
#include "gqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void for_each_h(tnode_t *node, size_t lvl, for_each_fn for_each_f) {
    for_each_f(&(tree_for_data){node, lvl});
}

static void bf_order(ktree_t *tree, for_each_fn for_each) {
    // TODO : if i wrote unit tests test it with ringbuffer
    queue_t lvlq = {0};
    queue_init(&lvlq, sizeof(tnode_t *));

    tnode_t *front = tree->root;
    if (front) enqueue(&lvlq, &front);
    while (!queue_empty(&lvlq)) {
        front = *(tnode_t **)queue_front(&lvlq);
        for_each_h(front, 0, for_each);
        for (size_t k = 0; k < tree->k; k++) {
            tnode_t *child = tnode_child(front, k);
            if (child) enqueue(&lvlq, &child);
        }
        dequeue(&lvlq);
    }
    queue_destroy(&lvlq);
}

static void in_order(tnode_t *node, size_t k, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    for (size_t n = 0; n < k; n++) {
        in_order(tnode_child(node, n), k, lvl + 1, for_each);
        if (n == (k - 1) / 2) for_each_h(node, lvl, for_each);
    }
}

static void post_order(tnode_t *node, size_t k, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    for (size_t n = 0; n < k; n++)
        post_order(tnode_child(node, n), k, lvl + 1, for_each);
    for_each_h(node, lvl, for_each);
}

static void pre_order(tnode_t *node, size_t k, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    for_each_h(node, lvl, for_each);
    for (size_t n = 0; n < k; n++)
        pre_order(tnode_child(node, n), k, lvl + 1, for_each);
}

static void (*order_functions[])(tnode_t *, size_t k, size_t lvl, for_each_fn) = {
    [PRE_ORDER] = pre_order,
    [IN_ORDER] = in_order,
    [POST_ORDER] = post_order,
};

static gdata_t kt_alloc(ktree_t *tree, size_t item_size, gdata_t data) {
    if (tree == NULL || tree == NULL) return NULL;
    gdata_t allocated =
        tree->allocator_fun ? tree->allocator_fun(item_size) : default_allocator(item_size);
    return memcpy(allocated, data, item_size);
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

static void kt_node_destroy(gdata_t for_data) {
    tree_for_data d = *(tree_for_data *)for_data;
    tnode_destroy(d.node);
    free(d.node);
}

void kt_destroy(ktree_t *tree) {
    kt_for_each(tree, POST_ORDER, kt_node_destroy);
}

void kt_for_each(ktree_t *tree, TRAVERSE_ORDER order, for_each_fn function) {
    if (order == BREADTH_FIRST_ORDER) {
        bf_order(tree, function);
    } else
        order_functions[order](tree->root, tree->k, 0, function);
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

// Binary tree

void tnode_rotate_left(tnode_t **node, size_t k) {
    tnode_t *right_child = tnode_child(*node, k - 1);
    if (!right_child) return;
    tnode_t *rchild_left_child = tnode_child(right_child, 0);
    tnode_set_child(*node, k - 1, NULL);
    tnode_set_child(right_child, 0, *node);
    if (rchild_left_child) {
        tnode_set_child(*node, k - 1, rchild_left_child);
    }
    *node = right_child;
}

void tnode_rotate_right(tnode_t **node, size_t k) {
    tnode_t *left_child = tnode_child(*node, 0);
    if (!left_child) return;
    tnode_t *lchild_right_child = tnode_child(left_child, 1);
    tnode_set_child(*node, 0, NULL);
    tnode_set_child(left_child, k - 1, *node);
    if (lchild_right_child) {
        tnode_set_child(*node, 0, lchild_right_child);
    }
    *node = left_child;
    // *parent = left_child;
}

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

    if (dir < 0) {
        tree->size--;
        return;
    }
    temp = tnode_child(node, dir);

    if (!temp) {
        tnode_t *new_node = tnode_create(tree->k);
        tnode_set_data(new_node, kt_alloc(tree, tree->item_size, data));
        tnode_set_child(node, dir, new_node);
        return;
    } else {
        bst_add_h(tree, temp, data);
    }
}

void bst_add(btree_t *tree, gdata_t data) {
    if (!tree->cmp_fun) {
        fprintf(stderr, "[ERROR]: there isn't compare function\n");
        return;
    }
    if (!tree->root) {
        tnode_t *new_node = tnode_create(tree->k);
        tnode_set_data(new_node, kt_alloc(tree, tree->item_size, data));
        tree->root = new_node;
    } else {
        bst_add_h(tree, tree->root, data);
    }
    tree->size++;
}

static tnode_t *bst_min_max(tnode_t *node, size_t direction) {
    // if direction == 0 return left most (minimum) child
    // if direction == 1 return right most (maximum) child
    tnode_t *temp = tnode_child(node, direction);
    if (temp) {
        return bst_min_max(temp, direction);
    } else
        return node;
}

static tnode_t *bst_right_min(tnode_t *node) {
    // return right child's max node
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
    tnode_set_child(parent, dir, 0);
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
            tnode_set_child(parent, dir, tnode_child(child, 0));
        else
            tnode_set_child(parent, dir, tnode_child(child, 1));
        tnode_destroy(child);

        free(child);
    } else {
        bst_delete_node(parent, child, dir);
    }
}

void bst_delete(btree_t *tree, gdata_t data) {
    if (tree->size == 0) return;
    tree->size--;
    bst_delete_h(tree, tree->root, data);
}

void bst_destroy(btree_t *tree) {
    kt_destroy(tree);
}
