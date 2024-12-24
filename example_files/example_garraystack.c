#include "garraystack.h"
#include <stdio.h>

int main() {

    astack_t stack = {0};
    astack_init(&stack, 10);
    // Set allocator currently not working
    /* astack_set_allocator(&stack, str_allocator); */
    astack_push_str(&stack, "hello");
    astack_push_str(&stack, "world");
    astack_push_str(&stack, "hello");
    astack_push_str(&stack, "world");
    astack_push_str(&stack, "hello");
    astack_push_str(&stack, "world");
    while (!astack_empty(&stack)) {
        printf("%s\n", (char *)astack_peak(&stack));
        astack_pop(&stack);
    }

    astack_destroy(&stack);
    return 0;
}
