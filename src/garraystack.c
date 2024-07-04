#include "../include/garraystack.h"
#include "../include/garraylist.h"
#include <stdint.h>
#include <stdlib.h>

struct Private {
    alist_t *buf;
};

astack_t *astack_create(size_t element_size) {
    astack_t *stack = (astack_t *)malloc(sizeof(astack_t));
    stack->pv = (Private *)malloc(sizeof(Private));
    stack->pv->buf = alist_create(element_size);
    return stack;
}

int16_t astack_push(astack_t *astack, gdata_t data) {
    return alist_push(astack->pv->buf, data);
}

int16_t astack_pop(astack_t *astack) {
    return alist_pop(astack->pv->buf);
}

size_t astack_size(astack_t *stack) {
    return alist_size(stack->pv->buf);
}

gdata_t astack_peak(astack_t *astack) {
    return alist_at(astack->pv->buf, alist_size(astack->pv->buf) - 1);
}

bool astack_empty(astack_t *astack) {
    return alist_empty(astack->pv->buf);
}

void astack_destroy(astack_t *astack) {
    alist_destroy(astack->pv->buf);
    free(astack->pv->buf);
    free(astack->pv);
}
