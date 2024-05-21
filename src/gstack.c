#include "../include/gstack.h"
#include <stdlib.h>

struct stack_t {
    llist_t *list;
};

stack_t *create_stack(void (*add_data)(node_t *, gdata_t data)) {
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    stack->list    = create_list(add_data);
    return stack;
}

int16_t stack_push(stack_t *stack, gdata_t data) {
    return push_front(stack->list, data);
}

void stack_pop(stack_t *stack) {
    pop_front(stack->list);
}

gdata_t stack_peak(stack_t *stack) {
    return peak_front(stack->list);
}

bool stack_is_empty(stack_t *stack) {
    return list_head(stack->list) == NULL ? true : false;
}

void destroy_stack(stack_t **stack) {
    destroy_list(&(*stack)->list);
    free(*stack);
    *stack = NULL;
}

void clear_stack(stack_t *stack) {
    while (!stack_is_empty(stack)) {
        stack_pop(stack);
    }
}
