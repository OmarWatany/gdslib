#include "garraylist.h"
#include <stdio.h>
#include <string.h>

void expand(alist_t *alist, size_t size);

alist_t *alist_create(size_t item_size) {
    alist_t *alist = (alist_t *)malloc(sizeof(alist_t));
    alist_init(alist, item_size);
    return alist;
}

int16_t alist_init(alist_t *alist, size_t item_size) {
    *alist = (alist_t){0};
    alist->capacity = 2;
    alist->item_size = item_size;
    alist->buf = (anode_t *)calloc(alist->capacity, sizeof(anode_t));
    return EXIT_SUCCESS;
}

alist_t alist_build_addr(size_t item_size, size_t arr_size, gdata_t arr[arr_size]) {
    alist_t alist = {0};
    alist_init(&alist, item_size);
    for (size_t j = 0; j < arr_size; j++)
        alist_push(&alist, arr[j]);
    return alist;
}

alist_t alist_build_num(size_t item_size, size_t arr_size, gnum_t arr[arr_size]) {
    alist_t alist = {0};
    alist_init(&alist, item_size);
    for (size_t j = 0; j < arr_size; j++)
        alist_push(&alist, &arr[j]);
    return alist;
}

alist_t alist_build_str(size_t arr_size, char *arr[arr_size]) {
    alist_t alist = {0};
    alist_init(&alist, 0);
    alist_set_allocator(&alist, str_allocator);
    for (size_t j = 0; j < arr_size; j++)
        alist_push(&alist, arr[j]);
    return alist;
}

int16_t alist_push(alist_t *alist, gdata_t data) {
    if (!alist) return EXIT_FAILURE;
    return alist_set_at(alist, alist->size, data);
    return EXIT_SUCCESS;
}

int16_t alist_push_safe(alist_t *alist, size_t item_size, gdata_t data) {
    if (!alist) return EXIT_FAILURE;
    return alist_set_at_safe(alist, alist->size, item_size, data);
    return EXIT_SUCCESS;
}

int16_t alist_pop(alist_t *alist) {
    if (!alist) return EXIT_FAILURE;
    return alist_rm_at(alist, alist_size(alist) - 1);
}

gdata_t alist_alloc(alist_t *alist, size_t item_size, gdata_t data) {
    if (alist == NULL || alist->buf == NULL) return NULL;
    return alist->allocator_fun ? alist->allocator_fun(data)
                                : default_safe_allocator(alist->item_size, item_size, data);
}

int16_t alist_set_at(alist_t *alist, size_t pos, gdata_t data) {
    return alist_set_at_safe(alist, pos, alist->item_size, data);
}

int16_t alist_set_at_safe(alist_t *alist, size_t pos, size_t item_size, gdata_t data) {
    if (alist == NULL || alist->buf == NULL) return EXIT_FAILURE;
    if (alist->size == alist->capacity) expand(alist, 5);
    if (pos >= alist->capacity) expand(alist, pos - alist->capacity + 1);

    if (!anode_data(&alist->buf[pos])) {
        gdata_t allocated = alist_alloc(alist, item_size, data);
        alist->size++;
        anode_set_data(&alist->buf[pos], allocated);
    } else
        anode_copy_data(&alist->buf[pos], alist->item_size, item_size, data);

    return EXIT_SUCCESS;
}

int16_t alist_rm_str_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL || pos >= alist->size) return EXIT_FAILURE;

    anode_t *node = &alist->buf[pos];
    anode_destroy(node);
    for (size_t i = pos + 1; i < alist->size; i++)
        alist->buf[i - 1] = alist->buf[i];
    anode_init(&alist->buf[alist->size - 1]);
    alist->size--;
    return EXIT_SUCCESS;
}

int16_t alist_rm_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL || pos >= alist->size) return EXIT_FAILURE;

    anode_t *node = &alist->buf[pos];
    if (node->data) {
        /* alist->deallocator_fun(node->data); */
        memset(node->data, 0, alist->item_size);
    }
    for (size_t i = pos + 1; i < alist->size; i++)
        alist->buf[i - 1] = alist->buf[i];
    anode_init(&alist->buf[alist->size - 1]);
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
    return alist->size;
}

size_t alist_item_size(alist_t *alist) {
    return alist->item_size;
}

size_t alist_capacity(alist_t *alist) {
    return alist->capacity;
}

void expand(alist_t *alist, size_t by) {
    if (!alist) return;
    alist->capacity += by;
    anode_t *neobuf = (anode_t *)realloc(alist->buf, sizeof(anode_t) * alist->capacity);
    if (neobuf) alist->buf = neobuf;
    for (size_t i = alist->size; i < alist->capacity; i++) {
        anode_init(&alist->buf[i]);
    }
}

void alist_purge(alist_t *alist) {
    if (alist == NULL) return;
    anode_t *temp = NULL;
    for (size_t i = 0; i < alist->capacity; i++) {
        if (!anode_data((temp = &alist->buf[i]))) continue;
        anode_destroy(temp);
    }
}

void alist_destroy(alist_t *alist) {
    alist_purge(alist);
    free(alist->buf);
}
