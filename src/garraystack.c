#include "../include/garraystack.h"
#include "../include/garraylist.h"
#include <stdint.h>
#include <stdlib.h>

astack_t *astack_create(size_t element_size) {
    astack_t *stack = (astack_t *)malloc(sizeof(astack_t));
    astack_init(stack, element_size);
    return stack;
}

int16_t astack_init(astack_t *stack, size_t item_size) {
    alist_init(&stack->buf, item_size);
    return EXIT_SUCCESS;
}

int16_t astack_push_safe(astack_t *stack, size_t item_size, gdata_t data) {
    return alist_push_safe(&stack->buf, item_size, data);
}

int16_t astack_push(astack_t *astack, gdata_t data) {
    return astack_push_safe(astack, alist_item_size(&astack->buf), data);
}

int16_t astack_pop(astack_t *astack) {
    return alist_pop(&astack->buf);
}

size_t astack_size(astack_t *stack) {
    return alist_size(&stack->buf);
}

gdata_t astack_peak(astack_t *astack) {
    return alist_at(&astack->buf, alist_size(&astack->buf) - 1);
}

bool astack_empty(astack_t *astack) {
    return alist_empty(&astack->buf);
}

void astack_set_allocator(astack_t *stack, allocator_fun_t allocator) {
    alist_set_allocator(&stack->buf, allocator);
}

void astack_destroy(astack_t *astack) {
    alist_destroy(&astack->buf);
}
