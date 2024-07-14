#ifndef ringbfr_H
#define ringbfr_H

#include "gallocator.h"
#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gds_types.h"
#include "gnode.h"
#include <stdbool.h>
#include <stdlib.h>

ringbuffer_t *ring_create(size_t item_size, size_t capacity);

int16_t ring_init(ringbuffer_t *ringbfr, size_t item_size, size_t capacity);
int16_t ring_write_safe(ringbuffer_t *list, size_t item_size, gdata_t data);
int16_t ring_write(ringbuffer_t *list, gdata_t data);
int16_t ring_overwrite_safe(ringbuffer_t *list, size_t item_size, gdata_t data);
int16_t ring_overwrite(ringbuffer_t *list, gdata_t data);
gdata_t ring_read(ringbuffer_t *list);
size_t  ring_size(ringbuffer_t *list);
size_t  ring_capacity(ringbuffer_t *list);
void    ring_set_allocator(ringbuffer_t *ringbfr, gdata_t (*allocator_fun)(gdata_t data));
bool    ring_empty(ringbuffer_t *list);
void    ring_destroy(ringbuffer_t *list);

#ifdef __cplusplus
}
#endif // cpp
#endif // !ringbfr_H
