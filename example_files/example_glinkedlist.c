#include "glinkedlist.h"
#include <stdio.h>
#include <string.h>

static void _print_str(gdata_t data);
static void _print_int(gdata_t data);

static void _print_int(gdata_t data) {
    int i = *(int *)data;
    printf("%d", i);
}

static inline void push_front_i(list_t *x, int y) {
    int i = y;
    push_front(x, &i);
}

static inline void push_back_i(list_t *x, int y) {
    int i = y;
    push_back(x, &i);
}

static inline void dump_str_list(list_t *x) {
    dump_list(x, _print_str);
    printf("\n");
}

static inline void dump_int_list(list_t *x) {
    dump_list(x, _print_int);
    printf("\n");
}

int main() {

    list_t *fixed_strings = list_create(30);
    list_set_allocator(fixed_strings, str_allocator);
    list_t *integers = list_create(sizeof(int));
    list_t *dyn_strings = list_create(3);
    list_set_allocator(dyn_strings, str_allocator);

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
        list_itr_t *fast_it = list_itr_create(fixed_strings);
        list_itr_t *slow_it = list_itr_create(fixed_strings);

        lnode_t *fast = itr_begin(fast_it);
        lnode_t *slow = itr_begin(slow_it);

        int length = 1, h_index = 0;
        while (fast != itr_end(fast_it)) {
            for (int i = 0; i < 2 && fast != itr_end(fast_it); i++) {
                length++;
                fast = next(fast_it);
            }
            slow = next(slow_it);
            h_index++;
            _print_str(lnode_data(slow));
        }
        printf("\n");
        printf("length = %d \n", length);
        printf("length / 2 = %d \n", h_index);

        free(fast_it);
        free(slow_it);
    }

    {
        list_itr_t *fast_it = list_itr_create(integers);
        lnode_t    *fast = itr_begin(fast_it);

        int length = 1, i = 0;
        while (fast != itr_end(fast_it)) {
            length++;
            i = *(int *)lnode_data(fast);
            printf("%d\n", i);
            fast = next(fast_it);
        }
        i = *(int *)lnode_data(fast);
        printf("%d\n", i);
        printf("integers length = %d \n", length);
        free(fast_it);
    }

    free(s);
    list_destroy(fixed_strings);
    free(fixed_strings);
    list_destroy(integers);
    free(integers);
    list_destroy(dyn_strings);
    free(dyn_strings);

    return 0;
}

void _print_str(gdata_t data) {
    printf("%s -> ", (char *)data);
}
