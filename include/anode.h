#ifndef ANODE_H
#define ANODE_H
#include <stdlib.h>

typedef void          *gdata_t;
typedef struct anode_t anode_t;

struct anode_t {
    size_t data_address;
};

int16_t init_node(anode_t *node);
int16_t anode_set_data(anode_t *node, gdata_t data);
gdata_t anode_data(anode_t *node);
void    free_anode(anode_t *list);
void    destroy_anode(anode_t **node);

#endif // !ANODE_H
