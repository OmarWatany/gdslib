#include "gstack.h"
#include "glinkedlist.h"
#include "string.h"
#include <stdlib.h>

stack_t *stack_create(size_t item_size) {
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    memset(stack, 0, sizeof(stack_t));
    list_init(&stack->list, item_size);
    return stack;
}

int16_t stack_init(stack_t *stack, size_t item_size) {
    list_init(&stack->list, item_size);
    return EXIT_SUCCESS;
}

int16_t stack_push_safe(stack_t *stack, size_t item_size, gdata_t data) {
    return push_front_safe(&stack->list, item_size, data);
}

int16_t stack_push(stack_t *stack, gdata_t data) {
    return push_front(&stack->list, data);
}

void stack_pop(stack_t *stack) {
    pop_front(&stack->list);
}

gdata_t stack_peak(stack_t *stack) {
    return peak_front(&stack->list);
}

bool stack_empty(stack_t *stack) {
    return stack->list.head == NULL ? true : false;
}

void stack_purge(stack_t *stack) {
    list_purge(&stack->list);
}

void stack_destroy(stack_t *stack) {
    list_destroy(&stack->list);
}
