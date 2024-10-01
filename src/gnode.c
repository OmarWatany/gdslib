#include "gnode.h"
#include <math.h>
#include <string.h>

gdata_t gnode_data(gdata_t node) {
    return ((gnode_t *)node)->data;
}

lnode_t *lnode_create() {
    lnode_t *new_node = (lnode_t *)malloc(sizeof(*new_node));
    lnode_init(new_node);
    return new_node;
}

int16_t lnode_init(lnode_t *node) {
    node->data = 0;
    return EXIT_SUCCESS;
}

gdata_t lnode_data(lnode_t *node) {
    if (!node) return NULL;
    return node->data;
}

int16_t lnode_set_data(lnode_t *node, gdata_t data) {
    if (!node) return EXIT_FAILURE;
    node->data = data;
    return EXIT_SUCCESS;
};

size_t lnode_link(lnode_t *node) {
    if (node == NULL) return 0;
    return node->link;
}

void lnode_set_link(lnode_t *node, uintptr_t new_link) {
    if (node == NULL) return;
    node->link = new_link;
}

void lnode_destroy(lnode_t *node) {
    if (!node) return;
    free(node->data);
}

tnode_t *tnode_create(size_t links_count) {
    tnode_t *new_node = (tnode_t *)malloc(sizeof(tnode_t));
    memset(new_node, 0, sizeof(tnode_t));
    tnode_init(new_node, links_count);
    return new_node;
}

int16_t tnode_init(tnode_t *node, size_t links_count) {
    size_t size = links_count * sizeof(tnode_t *);
    node->data = NULL;
    node->links = malloc(size);
    memset(node->links, 0, size);
    return EXIT_SUCCESS;
}

gdata_t tnode_data(tnode_t *node) {
    if (!node) return NULL;
    return node->data;
}

int16_t tnode_set_data(tnode_t *node, gdata_t data) {
    if (!node) return EXIT_FAILURE;
    node->data = data;
    return EXIT_SUCCESS;
};

tnode_t *tnode_child(tnode_t *node, size_t n) {
    if (!node || !node->links) return NULL;
    return node->links[n];
}

void tnode_set_child(tnode_t *node, size_t n, tnode_t *child) {
    if (!node) return;
    node->links[n] = child;
}

tnode_t **tnode_grand_children(tnode_t *node, int nk, size_t lvl) {
    int qc = pow(nk, lvl);
    int qfront = 0, qback = 0;

    tnode_t **childs = malloc(qc * sizeof(tnode_t *));
    tnode_t **lvlq = malloc(qc * sizeof(tnode_t *));
    memset(childs, 0, sizeof(tnode_t *) * qc);
    memset(lvlq, 0, sizeof(tnode_t *) * qc);

    tnode_t *temp = node;
    if (temp) lvlq[qback] = temp;
    qback = (qback + 1) % (qc);
    for (size_t i = 1; i <= lvl; i++) {
        for (size_t j = 0; j < pow(nk, i - 1); j++) {
            temp = lvlq[qfront];
            qfront = (qfront + 1) % (qc);
            for (int k = 0; k < nk; k++) {
                lvlq[qback] = tnode_child(temp, k);
                qback = (qback + 1) % (qc);
            }
        }
    }

    for (int j = 0; j < qc; j++) {
        childs[j] = lvlq[qfront];
        qfront = (qfront + 1) % (qc);
    }

    free(lvlq);
    return childs;
}

void tnode_destroy(tnode_t *node) {
    if (!node) return;
    free(node->links);
    free(node->data);
}
