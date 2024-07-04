#include "../include/gstack.h"
#include "../include/glinkedlist.h"
#include <stdlib.h>

stack_t *stack_create(size_t item_size) {
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    list_init(&stack->list, item_size);
    return stack;
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
    return list_head(&stack->list) == NULL ? true : false;
}

void stack_destroy(stack_t *stack) {
    list_item_size(&stack->list);
    list_destroy(&stack->list);
}

void clear_stack(stack_t *stack) {
    while (!stack_empty(stack)) {
        stack_pop(stack);
    }
}
