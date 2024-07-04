#include "../include/gringbuffer.h"
#include <stdio.h>
static int16_t irng_write(ringbuffer_t *ring, int i) {
    return ring_write(ring, &i);
}

static int16_t irng_overwrite(ringbuffer_t *ring, int i) {
    return ring_overwrite(ring, &i);
}

int main() {
    ringbuffer_t irng = {0};
    ring_init(&irng, sizeof(int), 10);

    for (size_t i = 0; i < ring_capacity(&irng); i++) {
        irng_write(&irng, i + 2);
    }

    printf("ring size : %zu\n", ring_size(&irng));
    printf("ring cap : %zu\n", ring_capacity(&irng));

    int res = irng_write(&irng, 23);
    if (res == EXIT_FAILURE) {
        printf("error : %m\n");
    }

    for (size_t i = 0; i < ring_capacity(&irng); i++) {
        gdata_t temp = ring_read(&irng);
        if (temp) printf("%ld -> %d \n", i, *(int *)temp);
    }

    printf("ring size : %zu\n", ring_size(&irng));
    printf("ring cap : %zu\n", ring_capacity(&irng));

    for (size_t i = 0; i < ring_capacity(&irng); i++) {
        irng_write(&irng, i);
    }

    // test this one
    res = irng_overwrite(&irng, 23);
    if (res == EXIT_FAILURE) {
        printf("error : %m\n");
    }

    for (size_t i = 0; !ring_empty(&irng); i++) {
        gdata_t temp = ring_read(&irng);
        if (temp) printf("%ld -> %d \n", i, *(int *)temp);
    }

    printf("ring size : %zu\n", ring_size(&irng));
    printf("ring cap : %zu\n", ring_capacity(&irng));

    ring_destroy(&irng);
    return EXIT_SUCCESS;
}
