#include "../include/gnode.h"
#include <string.h>

anode_t *anode_create() {
    anode_t *new_node = (anode_t *)malloc(sizeof(anode_t));
    memset(new_node, 0, sizeof(anode_t));
    return new_node;
}

int16_t anode_init(anode_t *node) {
    node->data_address = 0;
    return EXIT_SUCCESS;
}

gdata_t anode_data(anode_t *node) {
    if (!node) return NULL;
    return (gdata_t)node->data_address;
}

int16_t anode_set_data(anode_t *node, gdata_t data) {
    if (!node) return EXIT_FAILURE;
    node->data_address = (size_t)data;
    return EXIT_SUCCESS;
};

void anode_destroy(anode_t *node) {
    free((gdata_t)node->data_address);
}

lnode_t *lnode_create() {
    lnode_t *new_node = (lnode_t *)malloc(sizeof(lnode_t));
    memset(new_node, 0, sizeof(lnode_t));
    return new_node;
}

int16_t lnode_init(lnode_t *node) {
    node->data_address = 0;
    return EXIT_SUCCESS;
}

gdata_t lnode_data(lnode_t *node) {
    if (!node) return NULL;
    return (gdata_t)node->data_address;
}

int16_t lnode_set_data(lnode_t *node, gdata_t data) {
    if (!node) return EXIT_FAILURE;
    node->data_address = (size_t)data;
    return EXIT_SUCCESS;
};

size_t lnode_link(lnode_t *node) {
    if (node == NULL) return 0;
    return node->link;
}

void lnode_set_link(lnode_t *node, size_t new_link) {
    if (node == NULL) return;
    node->link = new_link;
}

void lnode_destroy(lnode_t *node) {
    if (!node) return;
    free((gdata_t)node->data_address);
}

knode_t *knode_create(size_t links_count) {
    knode_t *new_node = (knode_t *)malloc(sizeof(knode_t));
    memset(new_node, 0, sizeof(knode_t));
    new_node->links = malloc(links_count * sizeof(size_t));
    memset(new_node->links, 0, links_count * sizeof(size_t));
    return new_node;
}

int16_t knode_init(knode_t *node, size_t links_count) {
    node->data_address = 0;
    node->links = malloc(links_count * sizeof(size_t));
    return EXIT_SUCCESS;
}

gdata_t knode_data(knode_t *node) {
    if (!node) return NULL;
    return (gdata_t)node->data_address;
}

int16_t knode_set_data(knode_t *node, gdata_t data) {
    if (!node) return EXIT_FAILURE;
    node->data_address = (size_t)data;
    return EXIT_SUCCESS;
};

size_t knode_link(knode_t *node, size_t link_num) {
    if (node == NULL) return 0;
    return node->links[link_num];
}

void knode_set_link(knode_t *node, size_t link_num, size_t new_link) {
    if (node == NULL) return;
    node->links[link_num] = new_link;
}

void knode_destroy(knode_t *node) {
    if (!node) return;
    free(node->links);
    free((gdata_t)node->data_address);
}
