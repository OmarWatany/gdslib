#include "garraylist.h"
#include <stdio.h>
#include <string.h>

void expand(alist_t *alist, size_t size);
#define ALIST_AT(L, P)    ((L)->item_size * (P) + (L)->buf)
#define ALIST_BUF_SIZE(L) ((L) ? (L)->item_size * (L)->capacity : 0)

alist_t *alist_create(size_t item_size) {
    alist_t *alist = (alist_t *)malloc(sizeof(alist_t));
    alist_init(alist, item_size);
    return alist;
}

int16_t alist_init(alist_t *alist, size_t item_size) {
    *alist = (alist_t){0};
    alist->capacity = 2;
    alist->item_size = item_size;
    alist->buf = (anode_t *)calloc(alist->capacity, sizeof(anode_t) * item_size);
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
    return alist_push_safe(alist, alist->item_size, data);
}

int16_t alist_push_safe(alist_t *alist, size_t item_size, gdata_t data) {
    if (!alist || alist_set_at_safe(alist, alist->size, item_size, data)) return EXIT_FAILURE;
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
    if (pos >= alist->capacity) expand(alist, pos - alist->capacity + 1);

    // if item_size <= allocation_size
    // copy data to block
    // else if item_size > allocation_size
    // allocate memory for it and save pointer if item_size > sizeof(intptr_t)
    // save pointer in array node

    /* anode_t *node = alist->buf + pos * item_size; */
    anode_t *node = ALIST_AT(alist, pos);
    if (item_size <= alist->item_size) {
        memcpy(node, data, item_size);
    } else {
        // if i can't save the pointer value
        if (alist->item_size < sizeof(intptr_t)) return EXIT_FAILURE;
        // TODO : handle this case
        if (alist->allocator_fun) {
            gdata_t allocated = alist->allocator_fun(data);
            memcpy(allocated, data, item_size);
        }
    }

    return EXIT_SUCCESS;
}

int16_t alist_rm_str_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL || pos >= alist->size) return EXIT_FAILURE;
    memset(ALIST_AT(alist, pos), 0, alist->item_size);
    return EXIT_SUCCESS;
}

// NOTE: Should i have this
// it's good for dynamic string so i can deallocate them
// but i can't distinguish between dynamic allcoated values and the values saved in the array buffer
int16_t alist_rm_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL || pos >= alist->size) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

gdata_t alist_at(alist_t *alist, size_t pos) {
    if (alist == NULL || pos >= alist->size) return NULL;
    return ALIST_AT(alist, pos);
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

// NOTE: unsafe with dynamic allocated members
void alist_purge(alist_t *alist) {
    if (alist == NULL) return;
    memset(alist->buf, 0, ALIST_BUF_SIZE(alist));
    alist->size = 0;
}

void alist_destroy(alist_t *alist) {
    free(alist->buf);
}
