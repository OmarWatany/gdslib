#include "gringbuffer.h"

static void logBuffer(ringbuffer *buffer) {
    printf("----------\n");
    printf("Buffer Size  : %zu\n", buffer->size);
    printf("Buffer write : %zu\n", buffer->write_idx);
    printf("Buffer read  : %zu\n", buffer->read_idx);
    printf("buffer data  : %s\n", buffer->buffer);
    printf("----------\n");
}

int main() {
    char data[] = "hello";

    ringbuffer buffer = {0};
    ring_init(&buffer, sizeof data);
    printf("Data Size : %zu\n", sizeof data);
    logBuffer(&buffer);

    if (ring_write(&buffer, data, sizeof data)) {
        perror("couldn't write because of");
    }

    char *return_buffer = ring_read_return(&buffer, sizeof data - 1);
    if (!return_buffer)
        perror("couldn't read because of");
    else
        printf("%.*s\n", (int)(sizeof data - 1), return_buffer);

    logBuffer(&buffer);

    ring_reset(&buffer);
    logBuffer(&buffer);

    if (return_buffer) free(return_buffer);
    ring_destroy(&buffer);

    return 0;
}
