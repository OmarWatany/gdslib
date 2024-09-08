#include "../include/gds_types.h"
#include <string.h>

void itr_init(gitr_t *it, gdata_t ds) {
    it->ds = ds;
}

/*
gitr_t *itr_create(gdata_t ds) {
    gitr_t *it = malloc(sizeof(gitr_t));
    memset(it, 0, sizeof(gitr_t));
    itr_init(it, ds);
    return it;
}

void itr_set_end(gitr_t *it, gdata_t end) {
    if (!it) return;
    it->end = (uintptr_t)end;
}

void itr_set_begin(gitr_t *it, uintptr_t begin) {
    if (!it) return;
    it->begin = (uintptr_t)begin;
}

void itr_set_next(gitr_t *it, gnode_t *(*next)(gitr_t *iterator)) {
    if (!it) return;
    it->next = next;
}

void itr_set_prev(gitr_t *it, gnode_t *(*prev)(gitr_t *iterator)) {
    if (!it) return;
    it->prev = prev;
}

gnode_t *temp_next(gitr_t *it) {
    return it->next(it);
}

gnode_t *temp_prev(gitr_t *it) {
    return it->prev(it);
}
*/
