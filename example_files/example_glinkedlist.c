#include "../include/glinkedlist.h"
#include <stdio.h>
#include <string.h>

void _print_str(gdata_t data);
void _print_int(gdata_t data);

void _print_int(gdata_t data) {
    int i = *(int *)data;
    // int i = 8;
    printf("%d", i);
}

static inline void push_front_i(llist_t *x, int y) {
    int i = y;
    push_front(x, &i);
}

static inline void push_back_i(llist_t *x, int y) {
    int i = y;
    push_back(x, &i);
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

    llist_t *fixed_strings = create_list(30);
    llist_t *integers = create_list(sizeof(int));
    llist_t *dyn_strings = create_list(3);
    llist_set_allocator(dyn_strings, str_allocator);

    push_front(dyn_strings, "dyn strings");

    push_front_i(integers, 3);
    push_front_i(integers, 2);
    push_front_i(integers, 7);
    dump_int_list(integers);

    for (int i = 0; i < 7; i++) {
        push_front(fixed_strings, "world");
        push_front(fixed_strings, "hello");
        push_front(fixed_strings, "print");
        push_back(fixed_strings, "!");
    }
    char *s = strdup("strings");
    char *fs = "strings";

    push_front(fixed_strings, fs);
    // push_front(fixed_strings, s);
    //
    push_front(dyn_strings, s);
    push_back(dyn_strings, s);

    dump_str_list(fixed_strings);
    dump_str_list(dyn_strings);

    {
        list_iterator_t *fast_it = create_list_iterator(fixed_strings);
        list_iterator_t *slow_it = create_list_iterator(fixed_strings);
        node_t          *fast = itr_begin(fast_it);
        node_t          *slow = itr_begin(slow_it);

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

        free(fast_it);
        free(slow_it);
    }

    {
        list_iterator_t *fast_it = create_list_iterator(integers);
        node_t          *fast = itr_begin(fast_it);

        int length = 1, i = 0;
        while (fast != itr_end(fast_it)) {
            length++;
            i = *(int *)node_data(fast);
            printf("%d\n", i);
            fast = next(fast_it);
        }
        i = *(int *)node_data(fast);
        printf("%d\n", i);
        printf("integers length = %d \n", length);
        free(fast_it);
    }

    free(s);
    destroy_list(&fixed_strings);
    destroy_list(&integers);
    destroy_list(&dyn_strings);

    return 0;
}

void _print_str(gdata_t data) {
    printf("%s -> ", (char *)data);
}
