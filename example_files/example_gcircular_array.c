#include "../include/gcircular_array.h"
#include <stdio.h>

static int16_t irng_write(circular_array_t *carray, int i) {
    return carray_write(carray, &i);
}

static int16_t irng_overwrite(circular_array_t *carray, int i) {
    return carray_overwrite(carray, &i);
}

int main() {
    circular_array_t irng = {0};
    carray_init(&irng, sizeof(int), 10);

    for (size_t i = 0; i < carray_capacity(&irng); i++) {
        irng_write(&irng, i + 2);
    }

    printf("carray size : %zu\n", carray_size(&irng));
    printf("carray cap : %zu\n", carray_capacity(&irng));

    int res = irng_write(&irng, 23);
    if (res == EXIT_FAILURE) {
        printf("error : %m\n");
    }

    for (size_t i = 0; i < carray_capacity(&irng); i++) {
        gdata_t temp = carray_read(&irng);
        if (temp) printf("%ld -> %d \n", i, *(int *)temp);
    }

    printf("carray size : %zu\n", carray_size(&irng));
    printf("carray cap : %zu\n", carray_capacity(&irng));

    for (size_t i = 0; i < carray_capacity(&irng); i++) {
        irng_write(&irng, i);
    }

    // test this one
    res = irng_overwrite(&irng, 23);
    if (res == EXIT_FAILURE) {
        printf("error : %m\n");
    }

    for (size_t i = 0; !carray_empty(&irng); i++) {
        gdata_t temp = carray_read(&irng);
        if (temp) printf("%ld -> %d \n", i, *(int *)temp);
    }

    printf("carray size : %zu\n", carray_size(&irng));
    printf("carray cap : %zu\n", carray_capacity(&irng));

    carray_destroy(&irng);
    return EXIT_SUCCESS;
}
