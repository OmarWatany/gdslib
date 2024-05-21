#include "../include/garraylist.h"
#include <stdlib.h>
#include <string.h>

struct alist_t {
    gdata_t *buf;
    size_t   item_size, capacity, size;
    void     (*allocate_data)(gdata_t *, gdata_t);
};

void expand(alist_t *alist, size_t size);

alist_t *create_alist(size_t item_size, void (*_allocate_data)(gdata_t *node, gdata_t data)) {
    alist_t *alist       = (alist_t *)malloc(sizeof(alist_t));
    alist->allocate_data = _allocate_data;
    alist->item_size     = item_size;
    alist->capacity      = 2;
    alist->size          = 0;
    alist->buf           = (gdata_t *)malloc(item_size * alist->capacity * sizeof(gdata_t));
    return alist;
}

int16_t alist_set_at(alist_t *alist, size_t pos, gdata_t data) {
    if (alist == NULL || alist->buf == NULL)
        return EXIT_FAILURE;
    if (pos >= alist->capacity)
        expand(alist, pos - alist->capacity + 1);
    if (alist->size == alist->capacity)
        expand(alist, 5);
    gdata_t temp = NULL;
    alist->allocate_data(&temp, data);
    *(alist->buf + alist->size * alist->item_size) = temp;
    alist->size++;
    return EXIT_SUCCESS;
}

int16_t alist_rm_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL)
        return EXIT_FAILURE;
    alist->size--;
    free(*(pos * alist->item_size + alist->buf));
    *(pos * alist->item_size + alist->buf) = NULL;
    return EXIT_SUCCESS;
}

gdata_t alist_at(alist_t *alist, size_t pos) {
    if (alist == NULL || pos >= alist->size)
        return NULL;
    return *(pos * alist->item_size + alist->buf);
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
    gdata_t *neobuf = (gdata_t *)realloc(alist->buf, sizeof(gdata_t) * alist->capacity * alist->item_size);
    if (neobuf)
        alist->buf = neobuf;
}

void destroy_alist(alist_t **alist) {
    clear_alist(*alist);
    free(*alist);
    // memset(alist, 0, sizeof(alist_t));
}

void clear_alist(alist_t *alist) {
    if (alist == NULL)
        return;

    for (size_t i = 0; i < alist->size; i++) {
        free(*(i * alist->item_size + alist->buf));
    }

    free(alist->buf);
}
