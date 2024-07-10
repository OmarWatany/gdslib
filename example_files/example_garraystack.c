#include "../include/garraystack.h"
#include <stdio.h>
#include <string.h>

void _allocate_s(void **, void *);

int main() {

    // astack_t *stack = astack_create(sizeof(char *));
    astack_t stack = {0};
    astack_init(&stack, sizeof(char *));
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

void _allocate_s(void **node, void *data) {
    *node = strdup((char *)data);
}
