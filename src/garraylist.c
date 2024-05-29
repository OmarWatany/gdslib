#include "../include/garraylist.h"
#include <stdlib.h>
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

int16_t anode_set_data(anode_t *node, size_t item_size, gdata_t data) {
    gdata_t temp = malloc(item_size);
    if (!node || !temp)
        return EXIT_FAILURE;

    if (!memcpy(temp, data, item_size))
        return EXIT_FAILURE;

    node->data = temp;
    return EXIT_SUCCESS;
};

void destroy_anode(anode_t *node) {
    free(node->data);
    free(node);
}

struct alist_t {
    anode_t **buf;
    size_t    item_size, capacity, size;
};

void expand(alist_t *alist, size_t size);

alist_t *create_alist(size_t item_size) {
    alist_t *alist   = (alist_t *)malloc(sizeof(alist_t));
    alist->item_size = item_size;
    alist->capacity  = 2;
    alist->size      = 0;
    alist->buf       = (anode_t **)malloc(alist->capacity * sizeof(anode_t *));
    return alist;
}

int16_t alist_push(alist_t *list, gdata_t data) {
    if (!list)
        return EXIT_FAILURE;
    return alist_set_at(list, alist_size(list), data);
}
int16_t alist_pop(alist_t *list) {
    if (!list)
        return EXIT_FAILURE;
    return alist_rm_at(list, alist_size(list) - 1);
}

int16_t alist_set_at(alist_t *alist, size_t pos, gdata_t data) {
    if (alist == NULL || alist->buf == NULL)
        return EXIT_FAILURE;
    if (pos >= alist->capacity)
        expand(alist, pos - alist->capacity + 1);
    if (alist->size == alist->capacity)
        expand(alist, 5);

    alist->buf[alist->size] = create_anode();
    anode_set_data(alist->buf[alist->size], alist->item_size, data);

    alist->size++;
    return EXIT_SUCCESS;
}

int16_t alist_rm_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL || pos >= alist->size)
        return EXIT_FAILURE;
    alist->size--;
    destroy_anode(alist->buf[pos]);
    return EXIT_SUCCESS;
}

gdata_t alist_at(alist_t *alist, size_t pos) {
    if (alist == NULL || pos >= alist->size)
        return NULL;
    return anode_data(alist->buf[pos]);
}

void alist_reserve(alist_t *alist, size_t size) {
    if (!alist->buf) {

    } else if (alist->capacity < size)
        expand(alist, size - alist->capacity);
}

bool alist_is_empty(alist_t *alist) {
    if (alist == NULL || alist->buf == NULL || alist->size == 0)
        return true;
    return 0;
}

size_t alist_size(alist_t *alist) {
    return (int)alist->size;
}

size_t alist_capacity(alist_t *alist) {
    return (int)alist->capacity;
}

void expand(alist_t *alist, size_t size) {
    if (!alist)
        return;
    alist->capacity += size;
    anode_t **neobuf = (anode_t **)realloc(alist->buf, sizeof(anode_t *) * alist->capacity);
    if (neobuf)
        alist->buf = neobuf;
}

void destroy_alist(alist_t **alist) {
    clear_alist(*alist);
    free(*alist);
}

void clear_alist(alist_t *alist) {
    if (alist == NULL)
        return;

    for (size_t i = 0; i < alist->size; i++) {
        destroy_anode(alist->buf[i]);
    }

    free(alist->buf);
}
