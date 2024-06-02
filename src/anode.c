#include "../include/anode.h"
#include "../include/garraylist.h"
#include <string.h>

int16_t init_node(anode_t *node) {
    node->data_address = 0;
    return EXIT_SUCCESS;
}

int16_t anode_set_data(anode_t *node, gdata_t data) {
    if (!node) return EXIT_FAILURE;
    node->data_address = (size_t)data;
    return EXIT_SUCCESS;
};

gdata_t anode_data(anode_t *node) {
    if (!node) return NULL;
    return (gdata_t)node->data_address;
}

void free_anode(anode_t *node) {
    free((gdata_t)node->data_address);
}

void destroy_anode(anode_t **node) {
    free_anode(*node);
    free(*node);
    *node = NULL;
}
