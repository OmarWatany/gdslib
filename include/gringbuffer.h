#ifndef _GRINGBUFFER_H_
#define _GRINGBUFFER_H_

#define BUFFER_BACKEND_LIBC_MALLOC        0
#define BUFFER_BACKEND_LINUX_MMAP         1
#define BUFFER_BACKEND_WIN32_VIRTUALALLOC 2

#define BUFFER_BACKEND BUFFER_BACKEND_LIBC_MALLOC

#ifndef BUFFER_BACKEND
#if defined(_WIN32)
#define BUFFER_BACKEND BUFFER_BACKEND_WIN32_VIRTUALALLOC
#else
#define BUFFER_BACKEND BUFFER_BACKEND_LINUX_MMAP
#endif
#endif // BUFFER_BACKEND

#include "gds_types.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void *ringbuffer_alloc(size_t *);

void ring_init(ringbuffer *buffer, size_t size);

int ring_write(ringbuffer *buffer, gdata_t data, size_t size);

int ring_read(ringbuffer *buffer, void *target, size_t size);

gdata_t ring_read_return(ringbuffer *buffer, size_t size);

void ring_reset(ringbuffer *buffer);

void ring_destroy(ringbuffer *buffer);

#endif // _GRINGBUFFER_H_
