#include "../include/gstack.h"
#include <stdio.h>

int16_t push_int(stack_t *stack, int value);

int stack_peak_i(stack_t *s) {
    int *top = (int *)stack_peak(s);
    if (top != NULL) return *top;
    return 0;
}

int main() {
    stack_t *stack = create_stack(sizeof(int));

    push_int(stack, 23);
    stack_destroy(stack);

    push_int(stack, 3);
    push_int(stack, 2);

    while (!stack_is_empty(stack)) {
        printf("%d\n", stack_peak_i(stack));
        stack_pop(stack);
    }

    stack_destroy(stack);
    free(stack);
    return 0;
}

int16_t push_int(stack_t *stack, int value) {
    int d = value;
    return stack_push(stack, &d);
}
