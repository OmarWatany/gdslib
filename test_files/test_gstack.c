#include "../include/gstack.h"
#include <stdio.h>

void    _allocate_int(node_t *node, void *data);
int16_t push_int(stack_t *stack, int value);

int stack_peak_i(stack_t *s) {
    int *top = (int *)stack_peak(s);
    if (top != NULL)
        return *top;
    return 0;
}

int main() {
    stack_t *stack = create_stack(_allocate_int);

    push_int(stack, 23);
    clear_stack(stack);

    push_int(stack, 3);
    push_int(stack, 2);

    while (!stack_is_empty(stack)) {
        printf("%d\n", stack_peak_i(stack));
        stack_pop(stack);
    }

    destroy_stack(&stack);
    return 0;
}

int16_t push_int(stack_t *stack, int value) {
    int d = value;
    return stack_push(stack, &d);
}

void _allocate_int(node_t *node, void *data) {
    int *temp = (int *)malloc(sizeof(int));
    *temp     = *(int *)data;
    node_set_data(node, temp);
}
