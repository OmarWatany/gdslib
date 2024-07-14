#include "../include/garraystack.h"
#include <stdio.h>

int main() {

    astack_t stack = {0};
    astack_init(&stack, 0);
    astack_set_allocator(&stack, str_allocator);
    astack_push(&stack, "hello");
    astack_push(&stack, "world");
    astack_push(&stack, "hello");
    astack_push(&stack, "world");
    astack_push(&stack, "hello");
    astack_push(&stack, "world");
    while (!astack_empty(&stack)) {
        printf("%s\n", (char *)astack_peak(&stack));
        astack_pop(&stack);
    }

    astack_destroy(&stack);
    return 0;
}
