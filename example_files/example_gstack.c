#include "gstack.h"
#include <stdio.h>

int16_t stack_push_i(stack_t *stack, int value);
int     stack_peak_i(stack_t *s);

int main() {
    stack_t *stack = stack_create(sizeof(int));

    stack_push_i(stack, 23);

    stack_push_i(stack, 3);
    stack_push_i(stack, 2);

    while (!stack_empty(stack)) {
        printf("%d\n", stack_peak_i(stack));
        stack_pop(stack);
    }

    stack_destroy(stack);
    free(stack);
    return 0;
}

int16_t stack_push_i(stack_t *stack, int value) {
    int d = value;
    return stack_push(stack, &d);
}

int stack_peak_i(stack_t *s) {
    int *top = (int *)stack_peak(s);
    if (top != NULL) return *top;
    return 0;
}
