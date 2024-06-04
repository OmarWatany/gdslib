#ifndef GNODE_H
#define GNODE_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include "gnodes_types.h"

anode_t *anode_create();
int16_t  anode_init(anode_t *node);
gdata_t  anode_data(anode_t *node);
int16_t  anode_set_data(anode_t *node, gdata_t data);
void     anode_destroy(anode_t *node);

lnode_t *lnode_create();
int16_t  lnode_init(lnode_t *node);
gdata_t  lnode_data(lnode_t *node);
int16_t  lnode_set_data(lnode_t *node, gdata_t data);
size_t   lnode_link(lnode_t *node);
void     lnode_set_link(lnode_t *node, size_t new_link);
void     lnode_destroy(lnode_t *node);

knode_t *knode_create(size_t links_count);
int16_t  knode_init(knode_t *node, size_t links_count);
gdata_t  knode_data(knode_t *node);
int16_t  knode_set_data(knode_t *node, gdata_t data);
size_t   knode_link(knode_t *node, size_t link_num);
void     knode_set_link(knode_t *node, size_t link_num, size_t new_link);
void     knode_destroy(knode_t *node);

#ifdef __cplusplus
}
#endif // cpp
#endif // GNODE_H
