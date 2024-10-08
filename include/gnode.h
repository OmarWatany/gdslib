#ifndef GNODE_H
#define GNODE_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gnode_macros.h"
#include "gnodes_types.h"
#include <stdbool.h>
#include <stdlib.h>

#define gnode_data(N) ((N) ? ((gnode_t *)N)->data : NULL)

anode_t *anode_create();
void     anode_copy_data(anode_t *node, size_t allocation_size, size_t item_size, gdata_t from);

lnode_t *lnode_create();

tnode_t  *tnode_create(size_t links_count);
int16_t   tnode_init(tnode_t *node, size_t links_count);
tnode_t **tnode_grand_children(tnode_t *node, int nk, size_t lvl);

#ifdef __cplusplus
}
#endif // cpp
#endif // GNODE_H
