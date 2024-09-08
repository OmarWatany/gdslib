#ifndef GNODE_T
#define GNODE_T

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include <stdint.h>
#include <stdlib.h>

typedef void *gdata_t;

typedef struct {
    gdata_t data;
} gnode_t;

typedef gnode_t anode_t;

typedef struct {
    gdata_t   data;
    uintptr_t link;
} lnode_t;

typedef struct tnode_t  tnode_t;
typedef struct dtnode_t dtnode_t;

struct tnode_t {
    gdata_t   data;
    tnode_t **links;
};

struct dtnode_t {
    gdata_t    data;
    dtnode_t **to, *from;
};

#ifdef __cplusplus
}
#endif // cpp
#endif // GNODE_T
