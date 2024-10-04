#include "gds_types.h"
#include "gitr.h"
#include "glinkedlist.h"
#include <stdio.h>
#include <string.h>

static void _print_int(gdata_t data) {
    int i = *(int *)data;
    printf("%d", i);
}

static void _print_str(gdata_t data) {
    printf("%s -> ", (char *)data);
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

void fixed_strings() {
    list_t *fixed_strings = list_create(30);

    for (int i = 0; i < 7; i++) {
        push_front(fixed_strings, "world");
        push_front(fixed_strings, "hello");
        push_front(fixed_strings, "print");
        push_back(fixed_strings, "!");
    }
    /* char *s = strdup("strings"); */
    /*  push_front(fixed_strings, s); */
    char *fs = "strings";
    push_front(fixed_strings, fs);

    printf("------------------- ");
    printf("fixed_strings\n");
    dump_str_list(fixed_strings);
    {
        printf("------------------- ");
        printf("Fixed_strings itrator\n");

        gitr_t  fast_it = list_gitr(fixed_strings);
        gitr_t  slow_it = list_gitr(fixed_strings);
        gdata_t fast = itr_begin(&fast_it);
        gdata_t slow = itr_begin(&slow_it);

        int length = 0, h_index = 0;
        while (fast != itr_end(&fast_it)) {
            for (int i = 0; i < 2 && fast != itr_end(&fast_it); i++) {
                if (fast) length++;
                fast = itr_next(&fast_it);
            }
            if (!fast) break;
            h_index++;
            _print_str(slow);
            slow = itr_next(&slow_it);
        }
        printf("\n");
        printf("length = %d \n", length);
        printf("length / 2 = %d \n", h_index);

        gitr_destroy(&fast_it);
        gitr_destroy(&slow_it);
    }

#if 1
    printf("------------------- ");
    printf("Destroying fixed_strings\n");
    char *temp = NULL;
    while ((temp = strpop_front(fixed_strings))) {
        printf("%s -> ", temp);
        free(temp);
    }
    printf("\n");
#else
    list_destroy(fixed_strings);
#endif
    free(fixed_strings);
}

void dyn_strings() {
    list_t *dyn_strings = list_create(3);
    list_set_allocator(dyn_strings, str_allocator);

    push_front(dyn_strings, "dyn strings");

    char *s = strdup("strings");
    push_front(dyn_strings, s);
    push_back(dyn_strings, s);

    printf("------------------- ");
    printf("Dynamic Strings\n");
    dump_str_list(dyn_strings);
    reverse_dump_list(dyn_strings, _print_str);
    printf("\n");

#if 0
    printf("Destroying Dynamic Strings\n");
    char *temp = 0;
    while ((temp = pop_front(dyn_strings)))
        printf("%s -> ", temp);
    printf("\n");
#else
    list_destroy(dyn_strings);
#endif
    free(dyn_strings);
    free(s);
}

void integer_list() {
    list_t *integers = list_create(sizeof(int));

    push_front_i(integers, 3);
    push_front_i(integers, 2);
    push_front_i(integers, 7);
    printf("------------------- ");
    printf("Integers \n");
    dump_int_list(integers);

    {
        printf("------------------- ");
        printf("Integers iterator\n");
        gitr_t fast_itr = list_gitr(integers);
        gitr_t slow_itr = list_gitr(integers);

        gdata_t fast = itr_begin(&fast_itr);
        gdata_t slow = itr_begin(&slow_itr);

        int length = 0, h_index = 0;
        while (fast != itr_end(&fast_itr)) {
            for (int i = 0; i < 2; i++) {
                if (fast) {
                    length++;
                    _print_int(fast);
                }
                fast = itr_next(&fast_itr);
            }
            if (!fast) break;
            slow = itr_next(&slow_itr);
            h_index++;
        }
        printf("\nCenter value: ");
        _print_int(slow);
        printf("\n");
        printf("length = %d \n", length);
        printf("length / 2 = %d \n", h_index);

        do {
            fast = itr_prev(&fast_itr);
            _print_int(fast);
        } while (fast != itr_begin(&fast_itr));
        printf("\n");

        gitr_destroy(&fast_itr);
        gitr_destroy(&slow_itr);
    }

    {
        printf("------------------- ");
        printf("Integers itrator\n");

        gitr_t  fast_it = list_gitr(integers);
        gdata_t fast = itr_begin(&fast_it);

        int length = 0, i = 0;
        while (fast != itr_end(&fast_it)) {
            length++;
            i = *(int *)fast;
            printf("%d ", i);
            fast = itr_next(&fast_it);
        }
        printf("\nlength = %d \n", length);
        gitr_destroy(&fast_it);
    }

    list_destroy(integers);
    free(integers);
}

int main() {
    fixed_strings();
    integer_list();
    dyn_strings();
    return 0;
}
