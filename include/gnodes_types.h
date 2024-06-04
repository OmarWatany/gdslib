#ifndef GNODE_T
#define GNODE_T

#ifdef __cplusplus
extern "C" {
#endif // cpp

#include <stdlib.h>

typedef void *gdata_t;

typedef struct {
    size_t data_address;
} anode_t;

typedef struct {
    size_t data_address;
    size_t link;
} lnode_t;

typedef struct {
    size_t  data_address;
    size_t *links;
} knode_t;

#ifdef __cplusplus
}
#endif // cpp
#endif // GNODE_T
