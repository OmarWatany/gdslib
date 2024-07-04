#include "../include/gringbuffer.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

struct Private {
    anode_t *buffer;
    size_t   read_pointer, write_pointer, capacity, size, item_size;
    gdata_t (*allocator_fun)(gdata_t data);
};

ringbuffer_t *ring_create(size_t item_size, size_t capacity) {
    ringbuffer_t *ring = (ringbuffer_t *)malloc(sizeof(ringbuffer_t));
    if (!ring) return NULL;
    ring_init(ring, item_size, capacity);
    return ring;
}

int16_t ring_init(ringbuffer_t *ring, size_t item_size, size_t capacity) {
    if (!ring) return EXIT_FAILURE;
    ring->pv = malloc(sizeof(Private));
    if (!ring->pv) return EXIT_FAILURE;
    memset(ring->pv, 0, sizeof(Private));
    ring->pv->capacity = capacity;
    ring->pv->item_size = item_size;
    ring->pv->buffer = (anode_t *)malloc(capacity * sizeof(anode_t));
    if (!ring->pv->buffer) return EXIT_FAILURE;
    // I can re allocate it after creating the ring
    // good use of batch_alloc or for each
    for (size_t i = 0; i < capacity; i++) {
        anode_init(&ring->pv->buffer[i]);
    }
    return EXIT_SUCCESS;
}

gdata_t ring_alloc(ringbuffer_t *ring, size_t item_size, gdata_t data) {
    if (ring == NULL || ring->pv->buffer == NULL) return NULL;
    return ring->pv->allocator_fun ? ring->pv->allocator_fun(data)
                                   : default_safe_allocator(ring->pv->item_size, item_size, data);
}

int16_t ring_write_safe(ringbuffer_t *ring, size_t item_size, gdata_t data) {
    size_t *wrptr = &ring->pv->write_pointer;
    if ((*wrptr + 1) % ring->pv->capacity == ring->pv->read_pointer) {
        errno = ENOBUFS;
        return EXIT_FAILURE;
    }
    gdata_t allocated = anode_data(&ring->pv->buffer[*wrptr]);
    if (!allocated) {
        allocated = ring_alloc(ring, item_size, data);
    } else
        memcpy(memset(allocated, 0, ring->pv->item_size), data, item_size);

    anode_set_data(&ring->pv->buffer[*wrptr], allocated);
    *wrptr = (*wrptr + 1) % ring->pv->capacity;
    ring->pv->size++;
    return EXIT_SUCCESS;
}

int16_t ring_write(ringbuffer_t *ring, gdata_t data) {
    return ring_write_safe(ring, ring->pv->item_size, data);
}

int16_t ring_overwrite_safe(ringbuffer_t *ring, size_t item_size, gdata_t data) {
    size_t *wrptr = &ring->pv->write_pointer;

    gdata_t allocated = anode_data(&ring->pv->buffer[*wrptr]);
    if (!allocated) {
        allocated = ring_alloc(ring, item_size, data);
    } else
        memcpy(memset(allocated, 0, ring->pv->item_size), data, item_size);
    anode_set_data(&ring->pv->buffer[*wrptr], allocated);
    *wrptr = (*wrptr + 1) % ring->pv->capacity;

    if (ring->pv->size < ring->pv->capacity)
        ring->pv->size++;
    else
        ring->pv->read_pointer = ring->pv->write_pointer;

    ring->pv->size++;
    return EXIT_SUCCESS;
}

int16_t ring_overwrite(ringbuffer_t *ring, gdata_t data) {
    return ring_overwrite_safe(ring, ring->pv->item_size, data);
}

gdata_t ring_read(ringbuffer_t *ring) {
    if (ring->pv->size == 0) {
        errno = ENODATA;
        return NULL;
    }
    size_t *rdptr = &ring->pv->read_pointer;
    gdata_t data = anode_data(&ring->pv->buffer[*rdptr]);
    *rdptr = (*rdptr + 1) % ring->pv->capacity;
    ring->pv->size--;
    return data;
}

size_t ring_size(ringbuffer_t *ring) {
    return ring->pv->size;
}

size_t ring_capacity(ringbuffer_t *ring) {
    return ring->pv->capacity;
}

void ring_set_allocator(ringbuffer_t *ring, gdata_t (*allocator_fun)(gdata_t data)) {
    ring->pv->allocator_fun = allocator_fun;
}

bool ring_empty(ringbuffer_t *ring) {
    return ring->pv->size == 0 ? true : false;
}

void ring_destroy(ringbuffer_t *ring) {
    for (size_t i = 0; i < ring->pv->capacity; i++) {
        if (anode_data(&ring->pv->buffer[i])) anode_destroy(&ring->pv->buffer[i]);
    }
    free(ring->pv->buffer);
    free(ring->pv);
}
