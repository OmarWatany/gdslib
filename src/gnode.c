#include "../include/gnode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
    size_t data_address;
    size_t link;
};

node_t *create_node() {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data_address = 0;
    new_node->link = 0;
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

void destroy_node(node_t **node) {
    free((gdata_t)(*node)->data_address);
    free(*node);
    *node = NULL;
}
