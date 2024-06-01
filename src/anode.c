#include "../include/garraylist.h"
#include <string.h>

struct anode_t {
    size_t data_address;
};

anode_t *create_anode() {
    anode_t *new_node = (anode_t *)malloc(sizeof(anode_t));
    new_node->data_address = 0;
    return new_node;
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

void destroy_anode(anode_t **node) {
    free((gdata_t)(*node)->data_address);
    free(*node);
    *node = NULL;
}
