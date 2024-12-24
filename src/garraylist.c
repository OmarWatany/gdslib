#include "garraylist.h"
#include "gallocator.h"
#include <stdio.h>
#include <string.h>

void expand(alist_t *alist, size_t size);
#define ALIST_AT(L, P)    ((L)->item_size * (P) + (L)->buf)
#define ALIST_BUF_SIZE(L) ((L)->item_size * (L)->capacity)
#define NODE_DDATA(N)     ((void *)(*(intptr_t *)(N)))

alist_t *alist_create(size_t item_size) {
    alist_t *alist = (alist_t *)malloc(sizeof(alist_t));
    alist_init(alist, item_size);
    return alist;
}

int16_t alist_init(alist_t *alist, size_t item_size) {
    *alist = (alist_t){0};
    alist->capacity = 2;
    alist->item_size = item_size;
    // TODO : move to set_at and if there is allocator use it
    alist->buf = (anode_t *)calloc(alist->capacity, sizeof(anode_t) * item_size);
    /* alist->gallocator = default_gallocator; */
    return EXIT_SUCCESS;
}

alist_t alist_build_addr(size_t item_size, size_t arr_size, gdata_t arr[arr_size]) {
    alist_t alist = {0};
    alist_init(&alist, item_size);
    for (size_t j = 0; j < arr_size; j++)
        alist_push(&alist, arr[j]);
    return alist;
}

alist_t alist_build_num(size_t item_size, size_t arr_size, size_t arr[arr_size]) {
    alist_t alist = {0};
    alist_init(&alist, item_size);
    for (size_t j = 0; j < arr_size; j++)
        alist_push(&alist, &arr[j]);
    return alist;
}

alist_t alist_build_str(size_t arr_size, char *arr[arr_size]) {
    alist_t alist = {0};
    alist_init(&alist, sizeof(char *));
    alist_set_allocator(&alist, str_allocator);
    alist.gallocator = default_gallocator;
    for (size_t j = 0; j < arr_size; j++)
        alist_dpush_str(&alist, arr[j]);
    return alist;
}

int16_t alist_push(alist_t *alist, gdata_t data) {
    return alist_push_safe(alist, alist->item_size, data);
}

int16_t alist_push_safe(alist_t *alist, size_t item_size, gdata_t data) {
    if (!alist || alist_set_at_safe(alist, alist->size, item_size, data)) return EXIT_FAILURE;
    alist->size++;
    return EXIT_SUCCESS;
}

int16_t alist_dpush(alist_t *alist, gdata_t data) {
    return alist_dpush_safe(alist, alist->item_size, data);
}

int16_t alist_dpush_safe(alist_t *alist, size_t item_size, gdata_t data) {
    if (!alist || alist_dset_at_safe(alist, alist->size, item_size, data)) return EXIT_FAILURE;
    alist->size++;
    return EXIT_SUCCESS;
}

int16_t alist_pop(alist_t *alist) {
    if (!alist) return EXIT_FAILURE;
    alist->size--;
    return EXIT_SUCCESS;
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
    if (pos >= alist->capacity) {
        expand(alist, alist->capacity);
        return alist_set_at_safe(alist, pos, item_size, data);
    }
    // if item_size <= allocation_size
    // copy data to block
    anode_t *node = ALIST_AT(alist, pos);
    if (item_size <= alist->item_size) memcpy(node, data, item_size);
    return EXIT_SUCCESS;
}

int16_t alist_dset_at(alist_t *alist, size_t pos, gdata_t data) {
    return alist_dset_at_safe(alist, pos, alist->item_size, data);
}

int16_t alist_dset_at_safe(alist_t *alist, size_t pos, size_t item_size, gdata_t data) {
    if (!alist || !alist->buf || alist->item_size < sizeof(intptr_t)) return EXIT_FAILURE;
    if (alist->size == alist->capacity) expand(alist, alist->capacity);
    if (pos >= alist->capacity) {
        expand(alist, alist->capacity);
        return alist_dset_at_safe(alist, pos, item_size, data);
    }
    gdata_t allocated = NULL;
    if (!alist->allocator_fun) {
        allocated = default_allocator(item_size, data);
    } else
        allocated = alist->allocator_fun(data);
    intptr_t *node = (intptr_t *)ALIST_AT(alist, pos);
    *node = (intptr_t)allocated;
    return EXIT_SUCCESS;
}

int16_t alist_rm_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL || pos >= alist->size) return EXIT_FAILURE;
    gdata_t node = ALIST_AT(alist, pos);
    if (!alist->gallocator || !alist->gallocator->deallocator) {
        memset(node, 0, alist->item_size);
    } else {
        alist->gallocator->deallocator(NODE_DDATA(node));
        memset(node, 0, alist->item_size);
    }
    return EXIT_SUCCESS;
}

gdata_t alist_at(alist_t *alist, size_t pos) {
    if (alist == NULL || pos >= alist->size) return NULL;
    void *node = ALIST_AT(alist, pos);
    bool  b =
        alist->allocator_fun != NULL || (alist->gallocator && alist->gallocator->allocator != NULL);
    return b ? NODE_DDATA(node) : node;
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
    anode_t *neobuf = (anode_t *)realloc(alist->buf, ALIST_BUF_SIZE(alist));
    if (neobuf) alist->buf = neobuf;
    memset(ALIST_AT(alist, alist->size), 0, (alist->capacity - alist->size) * alist->item_size);
}

void alist_purge(alist_t *alist) {
    if (alist == NULL) return;
    if (alist->gallocator && alist->gallocator->deallocator)
        for (size_t p = 0; p < alist->size; p++)
            alist_rm_at(alist, p);
    alist->size = 0;
    memset(alist->buf, 0, ALIST_BUF_SIZE(alist));
}

void alist_destroy(alist_t *alist) {
    alist_purge(alist);
    free(alist->buf);
}
