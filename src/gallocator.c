#include "../include/gallocator.h"
#include <string.h>

gdata_t default_allocator(gdata_t allocator_data) {
    size_t  item_size = ((allocator_data_t *)allocator_data)->item_size;
    gdata_t data = ((allocator_data_t *)allocator_data)->data;

    gdata_t *temp = malloc(item_size);
    return memcpy(temp, data, item_size);
}

gdata_t str_allocator(gdata_t str) {
    return strdup((char *)str);
}
