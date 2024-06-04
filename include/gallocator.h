#ifndef GALLOCATOR_T
#define GALLOCATOR_T

#include <stdlib.h>

typedef void *gdata_t;

gdata_t default_allocator(size_t item_size, gdata_t data);
gdata_t str_allocator(gdata_t str);

#endif // !GALLOCATOR_T
