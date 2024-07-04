#include "../include/garraylist.h"
#include "../include/gnode.h"
#include <stdio.h>
#include <string.h>

struct Private {
    anode_t *buf;
    size_t   item_size;
    gdata_t (*allocator_fun)(gdata_t data);
};

void expand(alist_t *alist, size_t size);

alist_t *alist_create(size_t item_size) {
    alist_t *alist = (alist_t *)malloc(sizeof(alist_t));
    alist->capacity = 2;
    alist->size = 0;
    alist->pv = malloc(sizeof(Private));
    alist->pv->item_size = item_size;
    alist->pv->allocator_fun = NULL;
    alist->pv->buf = (anode_t *)malloc(alist->capacity * sizeof(anode_t));
    memset(alist->pv->buf, 0, sizeof(size_t *) * alist->capacity);
    return alist;
}

int16_t alist_init(alist_t *alist, size_t item_size) {
    alist->pv->item_size = item_size;
    alist->capacity = 2;
    alist->size = 0;
    alist->pv->buf = (anode_t *)malloc(alist->capacity * sizeof(anode_t));
    memset(alist->pv->buf, 0, sizeof(anode_t) * alist->capacity);
    alist->pv->allocator_fun = NULL;
    return EXIT_SUCCESS;
}

int16_t alist_push(alist_t *alist, gdata_t data) {
    if (!alist) return EXIT_FAILURE;
    return alist_set_at(alist, alist->size, data);
    return EXIT_SUCCESS;
}

int16_t alist_push_safe(alist_t *list, size_t item_size, gdata_t data) {
    if (!list) return EXIT_FAILURE;
    return alist_set_at_safe(list, list->size, item_size, data);
    return EXIT_SUCCESS;
}

int16_t alist_pop(alist_t *list) {
    if (!list) return EXIT_FAILURE;
    return alist_rm_at(list, alist_size(list) - 1);
}

gdata_t alist_alloc(alist_t *alist, size_t item_size, gdata_t data) {
    if (alist == NULL || alist->pv->buf == NULL) return NULL;
    return alist->pv->allocator_fun ? alist->pv->allocator_fun(data)
                                    : default_safe_allocator(alist->pv->item_size, item_size, data);
}

int16_t alist_set_at(alist_t *alist, size_t pos, gdata_t data) {
    if (alist == NULL || alist->pv->buf == NULL) return EXIT_FAILURE;
    if (alist->size == alist->capacity) expand(alist, 5);
    if (pos >= alist->capacity) expand(alist, pos - alist->capacity + 1);

    gdata_t allocated = alist_alloc(alist, alist->pv->item_size, data);

    if (!anode_data(&alist->pv->buf[pos])) {
        anode_init(&alist->pv->buf[pos]);
        alist->size++;
    } else
        anode_destroy(&alist->pv->buf[pos]);

    anode_set_data(&alist->pv->buf[pos], allocated);

    return EXIT_SUCCESS;
}

int16_t alist_set_at_safe(alist_t *alist, size_t pos, size_t item_size, gdata_t data) {
    if (alist == NULL || alist->pv->buf == NULL) return EXIT_FAILURE;
    if (alist->size == alist->capacity) expand(alist, 5);
    if (pos >= alist->capacity) expand(alist, pos - alist->capacity + 1);

    gdata_t allocated = alist_alloc(alist, item_size, data);

    if (!anode_data(&alist->pv->buf[pos])) {
        anode_init(&alist->pv->buf[pos]);
        alist->size++;
    } else
        anode_destroy(&alist->pv->buf[pos]);

    anode_set_data(&alist->pv->buf[pos], allocated);

    return EXIT_SUCCESS;
}

int16_t alist_rm_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->pv->buf == NULL || pos >= alist->size) return EXIT_FAILURE;
    anode_destroy(&alist->pv->buf[pos]);

    for (size_t i = pos + 1; i < alist->size; i++)
        alist->pv->buf[i - 1] = alist->pv->buf[i];

    alist->size--;
    return EXIT_SUCCESS;
}

gdata_t alist_at(alist_t *alist, size_t pos) {
    if (alist == NULL || pos >= alist->size) return NULL;
    return anode_data(&alist->pv->buf[pos]);
}

void alist_reserve(alist_t *alist, size_t size) {
    if (!alist->pv->buf) return;
    if (alist->capacity < size) expand(alist, size - alist->capacity);
}

void alist_set_allocator(alist_t *alist, gdata_t (*allocator_fun)(gdata_t data)) {
    alist->pv->allocator_fun = allocator_fun;
}

bool alist_empty(alist_t *alist) {
    if (alist == NULL || alist->pv->buf == NULL || alist->size == 0) return true;
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
    anode_t *neobuf = (anode_t *)realloc(alist->pv->buf, sizeof(anode_t) * alist->capacity);
    if (neobuf) alist->pv->buf = neobuf;
    for (size_t i = alist->size; i < alist->capacity; i++) {
        anode_init(&alist->pv->buf[i]);
    }
}

void alist_destroy(alist_t *alist) {
    if (alist == NULL) return;
    for (size_t i = 0; i < alist->size; i++)
        anode_destroy(&alist->pv->buf[i]);
    free(alist->pv->buf);
    free(alist->pv);
}
