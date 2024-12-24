#include "gallocator.h"
#include <string.h>

Gallocator_t gallocator = (Gallocator_t){
    .deallocator = default_deallocator,
};

Gallocator_t *default_gallocator = &gallocator;

gdata_t default_allocator(size_t item_size, gdata_t data) {
    return default_safe_allocator(item_size, item_size, data);
}

gdata_t default_safe_allocator(size_t allocation_size, size_t item_size, gdata_t data) {
    gdata_t temp = malloc(allocation_size);
    memcpy(memset(temp, 0, allocation_size), data, item_size);
    return temp;
}

gdata_t str_allocator(gdata_t str) {
    return strdup((char *)str);
}

int default_deallocator(gdata_t data) {
    if (!data) return EXIT_FAILURE;
    free(data);
    return EXIT_SUCCESS;
}
