#include "../include/gtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define left(node) tnode_child(node, 0)
#define right(node) tnode_child(node, 1)

void bf_order(ktree_t *tree, for_each_fn for_each) {
    int qfront = 0, qback = 0;

    tnode_t **lvlq = malloc(tree->size * sizeof(tnode_t *));
    memset(lvlq, 0, sizeof(tnode_t *) * tree->size);

    tnode_t *temp = tree->root;
    if (temp) lvlq[qback] = temp;
    qback = (qback + 1) % (tree->size);
    for (size_t j = 0; j < tree->size; j++) {
        temp = lvlq[qfront];
        qfront = (qfront + 1) % (tree->size);
        for (int k = 0; k < tree->k; k++) {
            lvlq[qback] = tnode_child(temp, k);
            if (lvlq[qback]) {
                for_each(lvlq[qback], 0);
                qback = (qback + 1) % (tree->size);
            }
        }
    }
    free(lvlq);
}

void in_order(tnode_t *node, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    in_order(left(node), lvl + 1, for_each);
    for_each(node, lvl);
    in_order(right(node), lvl + 1, for_each);
}

void post_order(tnode_t *node, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    post_order(left(node), lvl + 1, for_each);
    post_order(right(node), lvl + 1, for_each);
    for_each(node, lvl);
}

void pre_order(tnode_t *node, size_t lvl, for_each_fn for_each) {
    if (node == NULL) return;
    for_each(node, lvl);
    pre_order(left(node), lvl + 1, for_each);
    pre_order(right(node), lvl + 1, for_each);
}

void (*order_functions[])(tnode_t *, size_t, for_each_fn) = {
    pre_order,
    in_order,
    post_order,
};

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

void kt_node_destroy(tnode_t *node, size_t lvl) {
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

// BST

btree_t *btr_create(size_t item_size) {
    ktree_t *tree = kt_create(item_size, 2);
    return tree;
}

void btr_init(ktree_t *tree, size_t item_size, int (*cmp_fun)(gdata_t data1, gdata_t data2)) {
    kt_init(tree, item_size, 2);
    tree->cmp_fun = cmp_fun;
}

void bst_rec_add(ktree_t *tree, tnode_t *node, gdata_t data) {
    tnode_t *temp = NULL;

    int res = tree->cmp_fun(tnode_data(node), data);
    int dir = -1;
    dir = res > 0 ? 0 : (res < 0 ? 1 : dir);

    if (dir < 0) return;
    temp = tnode_child(node, dir);

    if (!temp) {
        tnode_t *new_node = tnode_create(tree->k);
        if (tree->allocator_fun) {
            tnode_set_data(new_node, tree->allocator_fun(data));
        } else
            tnode_set_data(new_node, default_allocator(tree->item_size, data));
        tnode_set_link(node, dir, new_node);
        return;
    } else {
        bst_rec_add(tree, temp, data);
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
        bst_rec_add(tree, tree->root, data);
    tree->size++;
}

tnode_t *bst_min_max(tnode_t *node, size_t direction) {
    tnode_t *temp = tnode_child(node, direction);
    if (temp) {
        return bst_min_max(temp, direction);
    } else
        return node;
}

tnode_t *bst_right_min(tnode_t *node) {
    return bst_min_max(tnode_child(node, 1), 0);
}

gdata_t *bst_min(ktree_t *tree) {
    return tnode_data(bst_min_max(tree->root, 0));
}

gdata_t *bst_max(ktree_t *tree) {
    return tnode_data(bst_min_max(tree->root, 1));
}

// check on root outside this function so assume node is root's child
tnode_t *bst_find_parent(btree_t *heystack, tnode_t *parent, gdata_t needle) {
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

tnode_t *bst_find_h(btree_t *heystack, tnode_t *node, gdata_t needle) {
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

void bst_delete_node(tnode_t *parent, tnode_t *node, int dir) {
    tnode_destroy(node);
    free(node);
    tnode_set_link(parent, dir, 0);
}

void tnode_swap_data(tnode_t *from, tnode_t *to) {
    gdata_t temp = NULL;
    temp = to->data;
    to->data = from->data;
    from->data = temp;
}

void bst_delete_h(ktree_t *tree, tnode_t *node, gdata_t data) {

    tnode_t *parent = bst_find_parent(tree, node, data);
    if (!parent) return;
    tnode_t *child = bst_find_h(tree, parent, data);

    int top = 0, count = 0, dir = 0;

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
            tnode_set_link(parent, dir, child->links[0]);
        else
            tnode_set_link(parent, dir, child->links[1]);
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
