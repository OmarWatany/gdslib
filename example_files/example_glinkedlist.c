#include "../include/glinkedlist.h"
#include <stdio.h>
#include <string.h>

void _print_str(void *data);
void _print_int(void *data);

static inline void push_front_i(llist_t *x, int y) {
    int i = y;
    push_front(x, &i);
}

static inline void push_front_s(llist_t *x, const char *s) {
    if (strlen(s) <= list_item_size(x))
        push_front(x, (void *)s);
}

static inline void push_back_s(llist_t *x, const char *s) {
    if (strlen(s) <= list_item_size(x))
        push_back(x, (void *)s);
}

static inline void dump_str_list(llist_t *x) {
    dump_list(x, _print_str);
    printf("\n");
}

static inline void dump_int_list(llist_t *x) {
    dump_list(x, _print_int);
    printf("\n");
}

int main() {

    llist_t *strings  = create_list(30);
    llist_t *integers = create_list(sizeof(int));

    push_front_i(integers, 3);
    push_front_i(integers, 2);
    push_front_i(integers, 7);
    dump_int_list(integers);

    for (int i = 0; i < 7; i++) {
        push_front_s(strings, "world");
        push_front_s(strings, "hello");
        push_front_s(strings, "print");
        push_back_s(strings, "!");
    }

    dump_str_list(strings);

    list_iterator_t *fast_it = create_list_iterator(strings);
    list_iterator_t *slow_it = create_list_iterator(strings);

    node_t *fast = itr_begin(fast_it);
    node_t *slow = itr_begin(slow_it);

    {
        int length = 1, h_index = 0;
        while (fast != itr_end(fast_it)) {
            for (int i = 0; i < 2 && fast != itr_end(fast_it); i++) {
                length++;
                fast = next(fast_it);
            }
            slow = next(slow_it);
            h_index++;
            _print_str(node_data(slow));
        }
        printf("\n");
        printf("length = %d \n", length);
        printf("length / 2 = %d \n", h_index);
    }

    free(fast_it);
    free(slow_it);

    destroy_list(&strings);
    destroy_list(&integers);

    return 0;
}

void _print_str(void *data) {
    if (data == NULL)
        return;
    printf("%s -> ", (char *)data);
}

void _print_int(void *data) {
    if (data == NULL)
        return;
    printf("%d", *(int *)data);
}
