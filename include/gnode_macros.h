#ifndef GNODE_MACROS_H
#define GNODE_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif // cpp

#define node_data(N) ((N) ? ((gnode_t *)N)->data : NULL)

#define node_init(N)                                                                               \
    do {                                                                                           \
        if (!(N)) break;                                                                           \
        (N)->data = 0;                                                                             \
    } while (0);

#define node_set_data(N, D)                                                                        \
    do {                                                                                           \
        if (!(N)) break;                                                                           \
        (N)->data = (D);                                                                           \
    } while (0);

#define node_destroy(N)                                                                            \
    do {                                                                                           \
        if (!(N) || !((N)->data)) break;                                                           \
        free((N)->data);                                                                           \
        (N)->data = 0;                                                                             \
    } while (0);

// Linkedlist node
#define node_link(N) ((N) ? (N)->link : 0)

#define node_set_link(N, L)                                                                        \
    do {                                                                                           \
        if (!(N)) break;                                                                           \
        (N)->link = (L);                                                                           \
    } while (0)

// Tree node

#define tnode_child(N, n) ((N) ? (N)->links[(n)] : NULL)

#define tnode_set_child(N, n, C)                                                                   \
    do {                                                                                           \
        if (!(N)) break;                                                                           \
        (N)->links[(n)] = (C);                                                                     \
    } while (0)

#define tnode_destroy(N)                                                                           \
    do {                                                                                           \
        if (!(N) || !((N)->data)) break;                                                           \
        free((N)->data);                                                                           \
        free((N)->links);                                                                          \
        (N)->data = NULL;                                                                          \
    } while (0)

#ifdef __cplusplus
}
#endif // cpp
#endif // GNODE_MACROS_H
