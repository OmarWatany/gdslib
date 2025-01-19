#include "gallocator.h"

Gallocator_t gallocator = (Gallocator_t){
    .deallocator = default_deallocator,
};

Gallocator_t *default_gallocator = &gallocator;

gdata_t default_allocator(size_t item_size) {
    return malloc(item_size);
}

int default_deallocator(gdata_t data) {
    if (!data) return EXIT_FAILURE;
    free(data);
    return EXIT_SUCCESS;
}
