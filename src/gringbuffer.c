#include "gringbuffer.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define ringbuffer_check_remaining_space(B, S)                                                     \
    do {                                                                                           \
        if ((B)->size == 0 || (S) > ringbuffer_remaining_sapce(B)) {                               \
            errno = ENOBUFS;                                                                       \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
    } while (0);

#define ringbuffer_check_available_data(B, S)                                                      \
    do {                                                                                           \
        if (buffer->size == 0 || (S) > ringbuffer_available_data((B))) {                           \
            errno = ENODATA;                                                                       \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
    } while (0);

#if BUFFER_BACKEND == BUFFER_BACKEND_LINUX_MMAP

#include <sys/mman.h>

void *ringbuffer_alloc(size_t *MinSize) {
    size_t pageSize = getpagesize();
    // stolen from casey muratori's video (Powerful page mapping techniques)
    size_t sz = ((*MinSize + pageSize - 1)) & ~(pageSize - 1);

    *MinSize = sz;

    FILE *tmp = tmpfile();
    if (tmp == NULL) {
        perror("Failed to create temporary file");
        return 0;
    }
    int fd = fileno(tmp);
    // Set the file size to `sz`
    if (ftruncate(fd, sz) == -1) {
        perror("Failed to truncate temporary file");
        fclose(tmp); // Close file on failure
        return 0;
    }

    void *buffer = mmap(0, sz * 2, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buffer == MAP_FAILED) {
        perror("Failed to map the buffer");
        fclose(tmp);
    }

    if (mmap(buffer, sz, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0) == MAP_FAILED) {
        perror("Failed to map first half of the buffer");
        fclose(tmp);
        munmap(buffer, 2 * sz); // Clean up
        return NULL;
    }

    // Map the second half of the buffer
    if (mmap(buffer + sz, sz, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0) ==
        MAP_FAILED) {
        perror("Failed to map second half of the buffer");
        fclose(tmp);
        munmap(buffer, 2 * sz); // Clean up
        return NULL;
    }

    fclose(tmp);
    return buffer;
}

#elif BUFFER_BACKEND == BUFFER_BACKEND_WIN32_VIRTUALALLOC

#if !defined(_WIN32)
#error "Current platform is not Windows"
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void *buffer_alloc(size_t *MinSize) {
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    size_t pageSize = sys_info.dwPageSize;

    // Round the requested size to the nearest multiple of the page size
    size_t sz = ((*MinSize + pageSize - 1) & ~(pageSize - 1));
    *MinSize = sz;

    // Create a file mapping object backed by the system pagefile
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sz, NULL);
    if (hMapFile == NULL) {
        printf("Failed to create file mapping: %lu\n", GetLastError());
        return NULL;
    }

    // Map the memory once (first half of the buffer)
    void *buffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sz);
    if (buffer == NULL) {
        printf("Failed to map view of file (first mapping): %lu\n", GetLastError());
        CloseHandle(hMapFile);
        return NULL;
    }

    // Map the same memory again (second half of the buffer) in another address space
    void *buffer2 =
        MapViewOfFileEx(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sz, (LPVOID)((uintptr_t)buffer + sz));
    if (buffer2 == NULL) {
        printf("Failed to map view of file (second mapping): %lu\n", GetLastError());
        UnmapViewOfFile(buffer); // Clean up first mapping
        CloseHandle(hMapFile);
        return NULL;
    }

    // Close the file mapping handle, we don't need it anymore
    CloseHandle(hMapFile);

    return buffer;
}

#elif BUFFER_BACKEND == BUFFER_BACKEND_LIBC_MALLOC

void *ringbuffer_alloc(size_t *MinSize) {
    return calloc(*MinSize, 1);
}

#endif /* ifdef BUFFER_BACKEND */

void ring_init(ringbuffer *buffer, size_t size) {
    memset(buffer, 0, sizeof(ringbuffer));
    buffer->buffer = ringbuffer_alloc(&size);
    buffer->size = size;
}

int ring_write(ringbuffer *buffer, void *data, size_t size) {
    if (!buffer) return EXIT_FAILURE;
    ringbuffer_check_remaining_space(buffer, size);

#if BUFFER_BACKEND == BUFFER_BACKEND_LINUX_MMAP ||                                                 \
    BUFFER_BACKEND == BUFFER_BACKEND_WIN32_VIRTUALALLOC
    memcpy(ringbuffer_write_idx(buffer), data, size);
#elif BUFFER_BACKEND == BUFFER_BACKEND_LIBC_MALLOC
    size_t space_until_end = ringbuffer_space_until_end(buffer);
    if (size <= space_until_end) {
        memcpy(ringbuffer_write_idx(buffer), data, size);
    } else {
        memcpy(ringbuffer_write_idx(buffer), data, space_until_end);
        memcpy(buffer->buffer, data + space_until_end, size - space_until_end);
    }
#endif

    ringbuffer_commit_write(buffer, size);
    return EXIT_SUCCESS;
}

int ring_read(ringbuffer *buffer, void *target, size_t size) {
    if (!buffer || !target) return EXIT_FAILURE;
    ringbuffer_check_available_data(buffer, size);

#if BUFFER_BACKEND == BUFFER_BACKEND_LINUX_MMAP ||                                                 \
    BUFFER_BACKEND == BUFFER_BACKEND_WIN32_VIRTUALALLOC
    memcpy(target, ringbuffer_read_idx(buffer), size);
#elif BUFFER_BACKEND == BUFFER_BACKEND_LIBC_MALLOC
    size_t space_until_end = size - buffer->read_idx;
    if (size <= space_until_end) {
        memcpy(target, ringbuffer_read_idx(buffer), size);
    } else {
        memcpy(target, ringbuffer_read_idx(buffer), space_until_end);
        memcpy(target + space_until_end, ringbuffer_read_idx(buffer), size - space_until_end);
    }
#endif

    ringbuffer_commit_read(buffer, size);
    return EXIT_SUCCESS;
}

void *ring_read_return(ringbuffer *buffer, size_t size) {
    if (!buffer) return NULL;

    void *temp = NULL;
    if (!(temp = malloc(size))) return NULL;

    if (ring_read(buffer, temp, size) == EXIT_SUCCESS) return temp;
    free(temp);
    return NULL;
}

void ring_reset(ringbuffer *buffer) {
    if (!buffer) return;
    buffer->read_idx = buffer->write_idx = 0;
    memset(buffer->buffer, 0, buffer->size);
}

void ring_destroy(ringbuffer *buffer) {
    if (!buffer->buffer) return;
#if BUFFER_BACKEND == BUFFER_BACKEND_LINUX_MMAP
    munmap(buffer->buffer, buffer->size * 2);
#elif BUFFER_BACKEND == BUFFER_BACKEND_WIN32_VIRTUALALLOC
    // Unmap both views of the memory
    UnmapViewOfFile(buffer->buffer);
    UnmapViewOfFile(buffer->buffer + buffer->size);
#elif BUFFER_BACKEND == BUFFER_BACKEND_LIBC_MALLOC
    free(buffer->buffer);
#endif
}
