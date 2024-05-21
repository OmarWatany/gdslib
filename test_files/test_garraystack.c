#include "../include/garraystack.h"
#include <stdio.h>
#include <string.h>

void _allocate_s(void **, void *);

int main() {

    astack_t *stack = create_astack(sizeof(char *), _allocate_s);
    astack_push(stack, "hello");
    astack_push(stack, "world");
    while (!astack_is_empty(stack)) {
        printf("%s\n", (char *)astack_peak(stack));
        astack_pop(stack);
    }

    destroy_astack(&stack);
    return 0;
}

void _allocate_s(void **node, void *data) {
    *node = strdup((char *)data);
}
