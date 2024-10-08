#include "gnode.h"
#include "gnode_macros.h"
#include <string.h>

anode_t *anode_create() {
    anode_t *new_node = (anode_t *)malloc(sizeof(anode_t));
    node_init(new_node);
    return new_node;
}

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
