#include "../include/garraystack.h"
#include "../include/garraylist.h"
#include <stdint.h>
#include <stdlib.h>

struct astack_t {
    alist_t *buf;
    size_t  *top;
};

astack_t *create_astack(size_t element_size, void (*add_data)(gdata_t *node, gdata_t data)) {
    astack_t *stack = (astack_t *)malloc(sizeof(astack_t));
    stack->buf      = create_alist(element_size, add_data);
    return stack;
}

int16_t astack_push(astack_t *astack, gdata_t data) {
    if (!astack)
        return EXIT_FAILURE;
    return alist_set_at(astack->buf, alist_size(astack->buf), data);
}

int16_t astack_pop(astack_t *astack) {
    if (!astack)
        return EXIT_FAILURE;
    return alist_rm_at(astack->buf, alist_size(astack->buf) - 1);
}

gdata_t astack_peak(astack_t *astack) {
    return alist_at(astack->buf, alist_size(astack->buf) - 1);
}

bool astack_is_empty(astack_t *astack) {
    return alist_is_empty(astack->buf);
}

void destroy_astack(astack_t **astack) {
    alist_t *temp = (*astack)->buf;
    destroy_alist(&temp);
    free(*astack);
    *astack = NULL;
}

void clear_astack(astack_t *astack) {
    alist_t *temp = astack->buf;
    clear_alist(temp);
}
