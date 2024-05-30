typedef void *gdata_t;
#include <stdlib.h>

typedef struct {
    gdata_t data;
    size_t  item_size;
} allocator_data_t;

gdata_t default_allocator(gdata_t allocator_data);
gdata_t str_allocator(gdata_t str);
