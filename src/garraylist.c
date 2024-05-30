#include "../include/garraylist.h"
#include <string.h>

typedef struct {
    gdata_t data;
    size_t  item_size;
} allocator_data_t;

struct alist_t {
    anode_t **buf;
    size_t    item_size, capacity, size;
    gdata_t (*allocator_fun)(gdata_t data);
};

void    expand(alist_t *alist, size_t size);
gdata_t default_allocator(gdata_t allocator_data);

alist_t *create_alist(size_t item_size) {
    alist_t *alist = (alist_t *)malloc(sizeof(alist_t));
    alist->item_size = item_size;
    alist->capacity = 2;
    alist->size = 0;
    alist->buf = (anode_t **)malloc(alist->capacity * sizeof(anode_t *));

    alist->allocator_fun = NULL;
    return alist;
}

int16_t alist_push(alist_t *list, gdata_t data) {
    if (!list) return EXIT_FAILURE;
    return alist_set_at(list, alist_size(list), data);
}

int16_t alist_pop(alist_t *list) {
    if (!list) return EXIT_FAILURE;
    return alist_rm_at(list, alist_size(list) - 1);
}

int16_t alist_set_at(alist_t *alist, size_t pos, gdata_t data) {
    if (alist == NULL || alist->buf == NULL) return EXIT_FAILURE;
    if (pos >= alist->capacity) expand(alist, pos - alist->capacity + 1);
    if (alist->size == alist->capacity) expand(alist, 5);

    alist->buf[alist->size] = create_anode();
    gdata_t temp = NULL;
    if (alist->allocator_fun) {
        // use custom allocator
        temp = alist->allocator_fun(data);
    } else {
        // use default allocator
        allocator_data_t t = {
            data,
            alist->item_size,
        };
        temp = default_allocator(&t);
    }
    anode_set_data(alist->buf[alist->size], temp);

    alist->size++;
    return EXIT_SUCCESS;
}

int16_t alist_rm_at(alist_t *alist, size_t pos) {
    if (alist == NULL || alist->buf == NULL || pos >= alist->size) return EXIT_FAILURE;
    alist->size--;
    destroy_anode(&alist->buf[pos]);
    return EXIT_SUCCESS;
}

gdata_t alist_at(alist_t *alist, size_t pos) {
    if (alist == NULL || pos >= alist->size) return NULL;
    return anode_data(alist->buf[pos]);
}

void alist_reserve(alist_t *alist, size_t size) {
    if (!alist->buf) {

    } else if (alist->capacity < size)
        expand(alist, size - alist->capacity);
}

void alist_set_allocator(alist_t *alist, gdata_t (*allocator_fun)(gdata_t data)) {
    alist->allocator_fun = allocator_fun;
}

bool alist_is_empty(alist_t *alist) {
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
    anode_t **neobuf = (anode_t **)realloc(alist->buf, sizeof(anode_t *) * alist->capacity);
    if (neobuf) alist->buf = neobuf;
}

void destroy_alist(alist_t **alist) {
    clear_alist(*alist);
    free(*alist);
}

void clear_alist(alist_t *alist) {
    if (alist == NULL) return;

    for (size_t i = 0; i < alist->size; i++) {
        destroy_anode(&alist->buf[i]);
    }

    free(alist->buf);
}

gdata_t default_allocator(gdata_t allocator_data) {
    size_t  item_size = ((allocator_data_t *)allocator_data)->item_size;
    gdata_t data = ((allocator_data_t *)allocator_data)->data;

    gdata_t *temp = malloc(item_size);
    return memcpy(temp, data, item_size);
}
