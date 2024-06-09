#include "../include/garraylist.h"
#include "../include/gnode.h"
#include <stdio.h>
#include <string.h>

void expand(alist_t *alist, size_t size);

alist_t *alist_create(size_t item_size) {
    alist_t *alist = (alist_t *)malloc(sizeof(alist_t));
    alist->item_size = item_size;
    alist->capacity = 2;
    alist->size = 0;
    alist->buf = (anode_t *)malloc(alist->capacity * sizeof(anode_t));
    memset(alist->buf, 0, sizeof(size_t *) * alist->capacity);
    alist->allocator_fun = NULL;
    return alist;
}

int16_t alist_init(alist_t *alist, size_t item_size) {
    alist->item_size = item_size;
    alist->capacity = 2;
    alist->size = 0;
    alist->buf = (anode_t *)malloc(alist->capacity * sizeof(anode_t));
    memset(alist->buf, 0, sizeof(anode_t) * alist->capacity);
    alist->allocator_fun = NULL;
    return EXIT_SUCCESS;
}

int16_t alist_push(alist_t *alist, gdata_t data) {
    if (!alist) return EXIT_FAILURE;
    return alist_set_at(alist, alist->size, data);
    return EXIT_SUCCESS;
}

int16_t alist_pop(alist_t *list) {
    if (!list) return EXIT_FAILURE;
    return alist_rm_at(list, alist_size(list) - 1);
}

gdata_t alist_alloc(alist_t *alist, gdata_t data) {
    if (alist == NULL || alist->buf == NULL) return NULL;
    return alist->allocator_fun ? alist->allocator_fun(data)
                                : default_allocator(alist->item_size, data);
}

int16_t alist_set_at(alist_t *alist, size_t pos, gdata_t data) {
    if (alist == NULL || alist->buf == NULL) return EXIT_FAILURE;
    if (alist->size == alist->capacity) expand(alist, 5);
    if (pos >= alist->capacity) expand(alist, pos - alist->capacity + 1);

    gdata_t allocated = alist_alloc(alist, data);

    if (!anode_data(&alist->buf[pos])) {
        anode_init(&alist->buf[pos]);
        alist->size++;
    } else
        anode_destroy(&alist->buf[pos]);

    anode_set_data(&alist->buf[pos], allocated);

    return EXIT_SUCCESS;
}

int16_t alist_rm_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL || pos >= alist->size) return EXIT_FAILURE;
    anode_destroy(&alist->buf[pos]);

    for (size_t i = pos + 1; i < alist->size; i++)
        alist->buf[i - 1] = alist->buf[i];

    alist->size--;
    return EXIT_SUCCESS;
}

gdata_t alist_at(alist_t *alist, size_t pos) {
    if (alist == NULL || pos >= alist->size) return NULL;
    return anode_data(&alist->buf[pos]);
}

void alist_reserve(alist_t *alist, size_t size) {
    if (!alist->buf) return;
    if (alist->capacity < size) expand(alist, size - alist->capacity);
}

void alist_set_allocator(alist_t *alist, gdata_t (*allocator_fun)(gdata_t data)) {
    alist->allocator_fun = allocator_fun;
}

bool alist_empty(alist_t *alist) {
    if (alist == NULL || alist->buf == NULL || alist->size == 0) return true;
    return 0;
}

size_t alist_size(alist_t *alist) {
    return (int)alist->size;
}

size_t alist_capacity(alist_t *alist) {
    return (int)alist->capacity;
}

void expand(alist_t *alist, size_t size) {
    if (!alist) return;
    alist->capacity += size;
    anode_t *neobuf = (anode_t *)realloc(alist->buf, sizeof(anode_t) * alist->capacity);
    if (neobuf) alist->buf = neobuf;
    for (size_t i = alist->size; i < alist->capacity; i++) {
        anode_init(&alist->buf[i]);
    }
}

void alist_destroy(alist_t *alist) {
    if (alist == NULL) return;
    for (size_t i = 0; i < alist->size; i++)
        anode_destroy(&alist->buf[i]);
    free(alist->buf);
}
