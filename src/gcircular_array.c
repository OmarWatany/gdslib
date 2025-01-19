#include "gcircular_array.h"
#include "garraylist.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

circular_array_t *carray_create(size_t item_size, size_t capacity) {
    circular_array_t *carray = (circular_array_t *)malloc(sizeof(circular_array_t));
    if (!carray) return NULL;
    carray_init(carray, item_size, capacity);
    return carray;
}

int16_t carray_init(circular_array_t *carray, size_t item_size, size_t capacity) {
    if (!carray) return EXIT_FAILURE;
    carray->capacity = capacity;
    carray->item_size = item_size;
    // TODO: add the ability to use different allocator
    carray->buf = (anode_t *)calloc(capacity, item_size);
    if (!carray->buf) return EXIT_FAILURE;
    // I can re allocate it after creating the carray
    // good use of batch_alloc or for each
    return EXIT_SUCCESS;
}

gdata_t carray_alloc(circular_array_t *carray, size_t item_size, gdata_t data) {
    if (carray == NULL || carray->buf == NULL) return NULL;
    return carray->allocator_fun ? carray->allocator_fun(item_size) : default_allocator(item_size);
}

int16_t carray_write_safe(circular_array_t *carray, size_t item_size, gdata_t data) {
    size_t *wrptr = &carray->write_pointer;
    if ((*wrptr + 1) % carray->capacity == carray->read_pointer) {
        errno = ENOBUFS;
        return EXIT_FAILURE;
    }
    memcpy(carray->buf + *wrptr * carray->item_size, data, item_size);

    *wrptr = (*wrptr + 1) % carray->capacity;
    carray->size++;
    return EXIT_SUCCESS;
}

int16_t carray_write(circular_array_t *carray, gdata_t data) {
    return carray_write_safe(carray, carray->item_size, data);
}

int16_t carray_overwrite_safe(circular_array_t *carray, size_t item_size, gdata_t data) {
    size_t *wrptr = &carray->write_pointer;

    memcpy(carray->buf + *wrptr * carray->item_size, data, item_size);
    *wrptr = (*wrptr + 1) % carray->capacity;

    if (carray->size < carray->capacity)
        carray->size++;
    else
        carray->read_pointer = carray->write_pointer;

    carray->size++;
    return EXIT_SUCCESS;
}

int16_t carray_overwrite(circular_array_t *carray, gdata_t data) {
    return carray_overwrite_safe(carray, carray->item_size, data);
}

gdata_t carray_read(circular_array_t *carray) {
    if (carray->size == 0) {
        errno = ENODATA;
        return NULL;
    }
    size_t *rdptr = &carray->read_pointer;
    gdata_t data = carray->buf + *rdptr * carray->item_size;
    *rdptr = (*rdptr + 1) % carray->capacity;
    carray->size--;
    return data;
}

size_t carray_size(circular_array_t *carray) {
    return carray->size;
}

size_t carray_capacity(circular_array_t *carray) {
    return carray->capacity;
}

void carray_set_allocator(circular_array_t *carray, allocator_fun_t allocator_fun) {
    carray->allocator_fun = allocator_fun;
}

bool carray_empty(circular_array_t *carray) {
    return carray->size == 0 ? true : false;
}

void carray_destroy(circular_array_t *carray) {
    free(carray->buf);
}
