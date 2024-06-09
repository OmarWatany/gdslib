#include "../include/garraystack.h"
#include "../include/garraylist.h"
#include <stdint.h>
#include <stdlib.h>

astack_t *astack_create(size_t element_size) {
    astack_t *stack = (astack_t *)malloc(sizeof(astack_t));
    stack->buf = alist_create(element_size);
    return stack;
}

int16_t astack_push(astack_t *astack, gdata_t data) {
    return alist_push(astack->buf, data);
}

int16_t astack_pop(astack_t *astack) {
    return alist_pop(astack->buf);
}

gdata_t astack_peak(astack_t *astack) {
    return alist_at(astack->buf, alist_size(astack->buf) - 1);
}

bool astack_empty(astack_t *astack) {
    return alist_empty(astack->buf);
}

void astack_destroy(astack_t *astack) {
    alist_destroy(astack->buf);
    free(astack->buf);
}
