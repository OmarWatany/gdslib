typedef void *gdata_t;
#include <stdlib.h>

gdata_t default_allocator(size_t item_size, gdata_t data);
gdata_t str_allocator(gdata_t str);
