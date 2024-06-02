#include "../include/gnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_t *create_node() {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    memset(new_node, 0, sizeof(node_t));
    return new_node;
}

size_t node_link(node_t *node) {
    if (node == NULL) return 0;
    return node->link;
}

void node_set_link(node_t *node, size_t new_link) {
    if (node == NULL) return;
    node->link = new_link;
}

gdata_t node_data(node_t *node) {
    if (!node) return NULL;
    return (gdata_t)node->data_address;
}

int16_t node_set_data(node_t *node, gdata_t data) {
    if (!node) return EXIT_FAILURE;
    node->data_address = (size_t)data;
    return EXIT_SUCCESS;
};

void free_node(node_t *node) {
    if (!node) return;
    free((gdata_t)node->data_address);
}

void destroy_node(node_t **node) {
    if (!node || !*node) return;
    free_node(*node);
    free(*node);
    *node = NULL;
}
