#include <stdbool.h>
#include <stdlib.h>

#ifndef ASTACK_H
#define ASTACK_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

typedef void *gdata_t;

typedef struct astack_t astack_t;

astack_t *create_astack(size_t element_size, void (*_allocate_data)(gdata_t *node, gdata_t data));

int16_t astack_push(astack_t *stack, gdata_t data);
int16_t astack_pop(astack_t *stack);
void   *astack_peak(astack_t *stack);
bool    astack_is_empty(astack_t *stack);

void destroy_astack(astack_t **stack);
void clear_astack(astack_t *stack);

#ifdef __cplusplus
}
#endif // cpp
#endif // !STACK_H
