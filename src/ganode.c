#include "gnode.h"
#include <string.h>

anode_t *anode_create() {
    anode_t *new_node = (anode_t *)malloc(sizeof(anode_t));
    anode_init(new_node);
    return new_node;
}

void anode_init(anode_t *node) {
    node->data = 0;
}

gdata_t anode_data(anode_t *node) {
    if (!node) return NULL;
    return node->data;
}

void anode_set_data(anode_t *node, gdata_t data) {
    if (!node) return;
    node->data = data;
};

void anode_copy_data(anode_t *node, size_t allocation_size, size_t item_size, gdata_t from) {
    if (item_size > allocation_size) {
        gdata_t to = realloc(from, item_size);
        if (to)
            from = to;
        else
            return;
        allocation_size = item_size;
    }
    memcpy(memset(node->data, 0, allocation_size), from, item_size);
}

void anode_destroy(anode_t *node) {
    if (!node || !node->data) return;
    free(node->data);
    node->data = 0;
}
