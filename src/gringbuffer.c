#include "../include/gringbuffer.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

ringbuffer_t *ring_create(size_t item_size, size_t capacity) {
    ringbuffer_t *ring = (ringbuffer_t *)malloc(sizeof(ringbuffer_t));
    if (!ring) return NULL;
    ring_init(ring, item_size, capacity);
    return ring;
}

int16_t ring_init(ringbuffer_t *ring, size_t item_size, size_t capacity) {
    if (!ring) return EXIT_FAILURE;
    ring->capacity = capacity;
    ring->item_size = item_size;
    ring->buf = (anode_t *)malloc(capacity * sizeof(anode_t));
    if (!ring->buf) return EXIT_FAILURE;
    // I can re allocate it after creating the ring
    // good use of batch_alloc or for each
    for (size_t i = 0; i < capacity; i++)
        anode_init(&ring->buf[i]);
    return EXIT_SUCCESS;
}

gdata_t ring_alloc(ringbuffer_t *ring, size_t item_size, gdata_t data) {
    if (ring == NULL || ring->buf == NULL) return NULL;
    return ring->allocator_fun ? ring->allocator_fun(data)
                               : default_safe_allocator(ring->item_size, item_size, data);
}

int16_t ring_write_safe(ringbuffer_t *ring, size_t item_size, gdata_t data) {
    size_t *wrptr = &ring->write_pointer;
    if ((*wrptr + 1) % ring->capacity == ring->read_pointer) {
        errno = ENOBUFS;
        return EXIT_FAILURE;
    }
    gdata_t allocated = anode_data(&ring->buf[*wrptr]);
    if (!allocated) {
        allocated = ring_alloc(ring, item_size, data);
    } else
        memcpy(memset(allocated, 0, ring->item_size), data, item_size);

    anode_set_data(&ring->buf[*wrptr], allocated);
    *wrptr = (*wrptr + 1) % ring->capacity;
    ring->size++;
    return EXIT_SUCCESS;
}

int16_t ring_write(ringbuffer_t *ring, gdata_t data) {
    return ring_write_safe(ring, ring->item_size, data);
}

int16_t ring_overwrite_safe(ringbuffer_t *ring, size_t item_size, gdata_t data) {
    size_t *wrptr = &ring->write_pointer;

    gdata_t allocated = anode_data(&ring->buf[*wrptr]);
    if (!allocated) {
        allocated = ring_alloc(ring, item_size, data);
    } else
        memcpy(memset(allocated, 0, ring->item_size), data, item_size);
    anode_set_data(&ring->buf[*wrptr], allocated);
    *wrptr = (*wrptr + 1) % ring->capacity;

    if (ring->size < ring->capacity)
        ring->size++;
    else
        ring->read_pointer = ring->write_pointer;

    ring->size++;
    return EXIT_SUCCESS;
}

int16_t ring_overwrite(ringbuffer_t *ring, gdata_t data) {
    return ring_overwrite_safe(ring, ring->item_size, data);
}

gdata_t ring_read(ringbuffer_t *ring) {
    if (ring->size == 0) {
        errno = ENODATA;
        return NULL;
    }
    size_t *rdptr = &ring->read_pointer;
    gdata_t data = anode_data(&ring->buf[*rdptr]);
    *rdptr = (*rdptr + 1) % ring->capacity;
    ring->size--;
    return data;
}

size_t ring_size(ringbuffer_t *ring) {
    return ring->size;
}

size_t ring_capacity(ringbuffer_t *ring) {
    return ring->capacity;
}

void ring_set_allocator(ringbuffer_t *ring, gdata_t (*allocator_fun)(gdata_t data)) {
    ring->allocator_fun = allocator_fun;
}

bool ring_empty(ringbuffer_t *ring) {
    return ring->size == 0 ? true : false;
}

void ring_destroy(ringbuffer_t *ring) {
    for (size_t i = 0; i < ring->capacity; i++) {
        if (anode_data(&ring->buf[i])) anode_destroy(&ring->buf[i]);
    }
    free(ring->buf);
}
