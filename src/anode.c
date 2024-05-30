#include "../include/garraylist.h"
#include <string.h>

struct anode_t {
    gdata_t data;
};

anode_t *create_anode() {
    anode_t *new_node = (anode_t *)malloc(sizeof(anode_t));
    new_node->data    = NULL;
    return new_node;
}

gdata_t anode_data(anode_t *node) {
    if (!node)
        return NULL;
    return node->data;
}

int16_t anode_set_data(anode_t *node, gdata_t data) {
    if (!node)
        return EXIT_FAILURE;

    node->data = data;
    return EXIT_SUCCESS;
};

void destroy_anode(anode_t **node) {
    free((*node)->data);
    free(*node);
    *node = NULL;
}
